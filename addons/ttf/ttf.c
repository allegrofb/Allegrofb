#include "allegro5/allegro.h"
#ifdef ALLEGRO_CFG_OPENGL
#include "allegro5/allegro_opengl.h"
#endif
#include "allegro5/internal/aintern_vector.h"

#include "allegro5/allegro_ttf.h"
#include "allegro5/internal/aintern_ttf_cfg.h"
#include "allegro5/internal/aintern_dtor.h"
#include "allegro5/internal/aintern_system.h"

#include <ft2build.h>
#include FT_FREETYPE_H

/* Some low-end drivers enable automatic S3TC compression, which
 * requires glTexSubImage2D to only work on multiples of aligned
 * 4x4 pixel blocks with some buggy OpenGL drivers.
 * There's not much we can do about that in general - if the user
 * locks a portion of a bitmap not conformin to this it will fail
 * with such a driver.
 * 
 * However in many programs this is no problem at all safe for rendering
 * glyphs and simply aligning to 4 pixels here fixes it.
 */
#define ALIGN_TO_4_PIXEL

ALLEGRO_DEBUG_CHANNEL("font")

#ifdef ALIGN_TO_4_PIXEL
static INLINE int align4(int x)
{
   return (x + 3) & ~3;
}
#endif

typedef struct ALLEGRO_TTF_GLYPH_DATA
{
    ALLEGRO_BITMAP *bitmap;
    int x, y;
    int advance;
    bool monochrome;
} ALLEGRO_TTF_GLYPH_DATA;

typedef struct ALLEGRO_TTF_FONT_DATA
{
    FT_Face face;
    int glyphs_count;
    _AL_VECTOR cache_bitmaps;
    int cache_pos_x;
    int cache_pos_y;
    int cache_line_height;
    ALLEGRO_TTF_GLYPH_DATA *cache;
    int flags;
    bool no_premultiply_alpha;

    FT_StreamRec stream;
    ALLEGRO_FILE *file;
    unsigned long base_offset, offset;
} ALLEGRO_TTF_FONT_DATA;

static bool inited;
static FT_Library ft;
static ALLEGRO_FONT_VTABLE vt;

static int font_height(ALLEGRO_FONT const *f)
{
   ASSERT(f);
   return f->height;
}

static int font_ascent(ALLEGRO_FONT const *f)
{
    ALLEGRO_TTF_FONT_DATA *data;
    FT_Face face;

    ASSERT(f);

    data = f->data;
    face = data->face;

    return face->size->metrics.ascender >> 6;
}

static int font_descent(ALLEGRO_FONT const *f)
{
    ALLEGRO_TTF_FONT_DATA *data;
    FT_Face face;

    ASSERT(f);

    data = f->data;
    face = data->face;

    return (-face->size->metrics.descender) >> 6;
}

// FIXME: Add a special case for when a single glyph rendering won't fit
// into 256x256 pixels.
static void push_new_cache_bitmap(ALLEGRO_TTF_FONT_DATA *data)
{
    ALLEGRO_BITMAP **back;
    ALLEGRO_STATE state;

#ifdef ALLEGRO_CFG_OPENGL
    /* The FBO for the last bitmap is no longer required. */
    if (_al_vector_is_nonempty(&data->cache_bitmaps)) {
	back = _al_vector_ref_back(&data->cache_bitmaps);
	al_remove_opengl_fbo(*back);
    }
#endif

    back = _al_vector_alloc_back(&data->cache_bitmaps);

    /* The bitmap will be destroyed when the parent font is destroyed so
     * it is not safe to register a destructor for it.
     */
    _al_push_destructor_owner();
    *back = al_create_bitmap(256, 256);
    _al_pop_destructor_owner();

    /* Sometimes OpenGL will partly sample texels from the border of
     * glyphs. So we better clear the texture to transparency.
     */
    al_store_state(&state, ALLEGRO_STATE_BITMAP);
    al_set_target_bitmap(*back);
    al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
    al_restore_state(&state);
}

static ALLEGRO_BITMAP* create_glyph_cache(ALLEGRO_FONT const *f, int w,
   int h, bool new)
{
    ALLEGRO_TTF_FONT_DATA *data = f->data;
    ALLEGRO_BITMAP **p_cache;
    ALLEGRO_BITMAP *cache;
    ALLEGRO_BITMAP *ret;

    if (_al_vector_is_empty(&data->cache_bitmaps) || new) {
        push_new_cache_bitmap(data);
        data->cache_pos_x = 0;
        data->cache_pos_y = 0;
        data->cache_line_height = 0;
    }

    p_cache = _al_vector_ref_back(&data->cache_bitmaps);
    cache = *p_cache;
    
    #ifdef ALIGN_TO_4_PIXEL
    w = align4(w);
    h = align4(h);
    #endif

    if (data->cache_pos_x + w > al_get_bitmap_width(cache)) {
        data->cache_pos_y += data->cache_line_height + 2;
        #ifdef ALIGN_TO_4_PIXEL
        data->cache_pos_y = align4(data->cache_pos_y);
        #endif
        data->cache_pos_x = 0;
        data->cache_line_height = 0;
    }

    if (data->cache_pos_y + h > al_get_bitmap_height(cache)) {
        return create_glyph_cache(f, w, h, true);
    }

    ret = al_create_sub_bitmap(cache, data->cache_pos_x,
        data->cache_pos_y, w, h);
    data->cache_pos_x += w + 2;
    #ifdef ALIGN_TO_4_PIXEL
        data->cache_pos_x = align4(data->cache_pos_x);
    #endif
    if (h > data->cache_line_height)
        data->cache_line_height = h;
    return ret;
}


static int render_glyph(ALLEGRO_FONT const *f,
   ALLEGRO_COLOR color, int prev, int ch,
    float xpos, float ypos, ALLEGRO_TTF_GLYPH_DATA **measure_glyph)
{
    ALLEGRO_TTF_FONT_DATA *data = f->data;
    FT_Face face = data->face;
    int ft_index = FT_Get_Char_Index(face, ch);
    int advance = 0;

    ALLEGRO_TTF_GLYPH_DATA *glyph = data->cache + ft_index;
    if (!glyph->bitmap) {
        // FIXME: make this a config setting? FT_LOAD_FORCE_AUTOHINT
        int x, y, w, h;
        ALLEGRO_LOCKED_REGION *lr;
        ALLEGRO_STATE backup;
        FT_Error e;
        bool was_held;
        
        // FIXME: Investigate why some fonts don't work without the
        // NO_BITMAP flags. Supposedly using that flag makes small sizes
        // look bad so ideally we would not used it.
        e = FT_Load_Glyph(face, ft_index, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP |
           (glyph->monochrome ? FT_LOAD_TARGET_MONO : 0));
        if (e) {
           ALLEGRO_WARN("Failed loading glyph %d from.\n", ft_index);
        }
        w = face->glyph->bitmap.width;
        h = face->glyph->bitmap.rows;

        if (w == 0)
           w = 1;
        if (h == 0)
           h = 1;

        /* We must not change the target bitmap while holding drawing. */
        was_held = al_is_bitmap_drawing_held();
        al_hold_bitmap_drawing(false);
        al_store_state(&backup, ALLEGRO_STATE_BITMAP);

        al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);
        glyph->bitmap = create_glyph_cache(f, w, h, false);

        al_set_target_bitmap(glyph->bitmap);
        lr = al_lock_bitmap(glyph->bitmap, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_WRITEONLY);
        /* In case this is an empty bitmap, we need to at least draw
         * the one pixel we use as minimum size.
         * TODO: Does A5 not support bitmap with zero dimensions?
         * If it does, we can simplify this.
         */
        al_put_pixel(0, 0, al_map_rgba(0, 0, 0, 0));
        #ifdef ALIGN_TO_4_PIXEL
        {
           /* clear the extra borders we added */
           int i;
           int bw = al_get_bitmap_width(glyph->bitmap);
           int bh = al_get_bitmap_height(glyph->bitmap);
           for (i = 0; i < (bh - h); i++) {
              memset((unsigned char *)lr->data +
                 (bh - 1 - i) * lr->pitch, 0, 4 * bw);
           }
           for (i = 0; i < bh; i++) {
              memset((unsigned char *)lr->data +
                 i * lr->pitch + 4 * w, 0, 4 * (bw - w));
           }
        }
        #endif

        if (glyph->monochrome) {
           for (y = 0; y < face->glyph->bitmap.rows; y++) {
               unsigned char *ptr = face->glyph->bitmap.buffer + face->glyph->bitmap.pitch * y;
               unsigned char *dptr = (unsigned char *)lr->data + lr->pitch * y;
               int bit = 0;
               if (data->no_premultiply_alpha) {
                  for (x = 0; x < face->glyph->bitmap.width; x++) {
                     unsigned char set = ((*ptr >> (7-bit)) & 1) ? 255 : 0;
                     *dptr++ = 255;
                     *dptr++ = 255;
                     *dptr++ = 255;
                     *dptr++ = set;
                     bit++;
                     if (bit >= 8) {
                        bit = 0;
                        ptr++;
                     }
                  }
               }
               else {
                  for (x = 0; x < face->glyph->bitmap.width; x++) {
                     unsigned char set = ((*ptr >> (7-bit)) & 1) ? 255 : 0;
                     float setf = set / 255.0f;
                     *dptr++ = 255 * setf;
                     *dptr++ = 255 * setf;
                     *dptr++ = 255 * setf;
                     *dptr++ = set;
                     bit++;
                     if (bit >= 8) {
                        bit = 0;
                        ptr++;
                     }
                  }
               }
           }
        }
        else {
           for (y = 0; y < face->glyph->bitmap.rows; y++) {
               unsigned char *ptr = face->glyph->bitmap.buffer + face->glyph->bitmap.pitch * y;
               unsigned char *dptr = (unsigned char *)lr->data + lr->pitch * y;
               if (data->no_premultiply_alpha) {
                  for (x = 0; x < face->glyph->bitmap.width; x++) {
                     unsigned char c = *ptr;
                     *dptr++ = 255;
                     *dptr++ = 255;
                     *dptr++ = 255;
                     *dptr++ = c;
                     ptr++;
                  }
               }
               else {
                  for (x = 0; x < face->glyph->bitmap.width; x++) {
                     unsigned char c = *ptr;
                     float cf = c / 255.0f;
                     *dptr++ = 255 * cf;
                     *dptr++ = 255 * cf;
                     *dptr++ = 255 * cf;
                     *dptr++ = c;
                     ptr++;
                  }
               }
           }
        }

        al_unlock_bitmap(glyph->bitmap);
        glyph->x = face->glyph->bitmap_left;
        glyph->y = (face->size->metrics.ascender >> 6) - face->glyph->bitmap_top;
        glyph->advance = face->glyph->advance.x >> 6;

        al_restore_state(&backup);
        al_hold_bitmap_drawing(was_held);
    }

    /* Do kerning? */
    if (!(data->flags & ALLEGRO_TTF_NO_KERNING) && prev) {
        FT_Vector delta;
        FT_Get_Kerning(face, FT_Get_Char_Index(face, prev), ft_index,
            FT_KERNING_DEFAULT, &delta );
        advance += delta.x >> 6;
    }

    if (measure_glyph)
        *measure_glyph = glyph;
    else
        al_draw_tinted_bitmap(glyph->bitmap, color,
            xpos + glyph->x + advance, ypos + glyph->y, 0);

    advance += glyph->advance;

    return advance;
}

static int render_char(ALLEGRO_FONT const *f, ALLEGRO_COLOR color,
   int ch, float xpos,
   float ypos)
{
    return render_glyph(f, color, '\0', ch, xpos, ypos, NULL);
}

static int char_length(ALLEGRO_FONT const *f, int ch)
{
    ALLEGRO_TTF_GLYPH_DATA *glyph;
    ALLEGRO_COLOR dummy = {1, 1, 1, 1};
    return render_glyph(f, dummy, '\0', ch, 0, 0, &glyph);
}

static int render(ALLEGRO_FONT const *f, ALLEGRO_COLOR color,
   const ALLEGRO_USTR *text,
    float x, float y)
{
    int pos = 0;
    int advance = 0;
    int32_t prev = '\0';
    int32_t ch;
    bool on = al_is_bitmap_drawing_held();

    al_hold_bitmap_drawing(true);

    while ((ch = al_ustr_get_next(text, &pos)) >= 0) {
        advance += render_glyph(f, color, prev, ch, x + advance, y, NULL);
        prev = ch;
    }

    al_hold_bitmap_drawing(on);

    return advance;
}

static int text_length(ALLEGRO_FONT const *f, const ALLEGRO_USTR *text)
{
    int pos = 0;
    int32_t prev = '\0';
    int32_t ch;
    int x = 0;
    ALLEGRO_TTF_GLYPH_DATA *glyph;
    ALLEGRO_COLOR dummy = {1, 1, 1, 1};
    while ((ch = al_ustr_get_next(text, &pos)) >= 0) {
        x += render_glyph(f, dummy, prev, ch, x, 0, &glyph);
        prev = ch;
    }
    return x;
}

static void get_text_dimensions(ALLEGRO_FONT const *f,
   ALLEGRO_USTR const *text,
   int *bbx, int *bby, int *bbw, int *bbh)
{
    int32_t prev = '\0';
    int pos = 0;
    int i;
    int x = 0;
    ALLEGRO_TTF_GLYPH_DATA *glyph;
    ALLEGRO_COLOR dummy = {1, 1, 1, 1};

    int count = al_ustr_length(text);
    *bbx = 0;
    for (i = 0; i < count; i++) {
        int32_t ch = al_ustr_get_next(text, &pos);
        x += render_glyph(f, dummy, prev, ch, 0, 0, &glyph);
        if (i == count - 1) {
            x -= glyph->advance;
            x += glyph->x + al_get_bitmap_width(glyph->bitmap);
        }
        if (i == 0)
            *bbx = glyph->x;
        prev = ch;
    }
    *bby = 0; // FIXME
    *bbw = x - *bbx;
    *bbh = f->height; // FIXME, we want the bounding box!
}

#if 0
#include "allegro5/allegro_image.h"
static void debug_cache(ALLEGRO_FONT *f)
{
   ALLEGRO_TTF_FONT_DATA *data = f->data;
   _AL_VECTOR *v = &data->cache_bitmaps;
   static int j = 0;
   int i;
   
   al_init_image_addon();
   
   for (i = 0; i < (int)_al_vector_size(v); i++) {
      ALLEGRO_BITMAP **bmp = _al_vector_ref(v, i);
      ALLEGRO_USTR *u = al_ustr_newf("font%d.png", j++);
      al_save_bitmap(al_cstr(u), *bmp);
      al_ustr_free(u);
   }
}
#endif

static void destroy(ALLEGRO_FONT *f)
{
    int i;
    ALLEGRO_TTF_FONT_DATA *data = f->data;
    _AL_VECTOR *vec = &data->cache_bitmaps;
#if 0
    debug_cache(f);
#endif
    FT_Done_Face(data->face);
    for (i = 0; i < data->glyphs_count; i++) {
        if (data->cache[i].bitmap) {
            al_destroy_bitmap(data->cache[i].bitmap);
        }
    }
    while (_al_vector_is_nonempty(vec)) {
        ALLEGRO_BITMAP **bmp = _al_vector_ref_back(vec);
        al_destroy_bitmap(*bmp);
        _al_vector_delete_at(vec, _al_vector_size(vec)-1);
    }
    _al_vector_free(&data->cache_bitmaps);
    al_free(data->cache);
    al_free(data);
    al_free(f);
}

static unsigned long ftread(FT_Stream stream, unsigned long offset,
   unsigned char *buffer, unsigned long count)
{
    ALLEGRO_TTF_FONT_DATA *data = stream->pathname.pointer;
    unsigned long bytes;

    if (count == 0)
       return 0;

    if (offset != data->offset)
       al_fseek(data->file, data->base_offset + offset, ALLEGRO_SEEK_SET);
    bytes = al_fread(data->file, buffer, count);
    data->offset = offset + bytes;
    return bytes;
}

static void ftclose(FT_Stream  stream)
{
    ALLEGRO_TTF_FONT_DATA *data = stream->pathname.pointer;
    al_fclose(data->file);
    data->file = NULL;
}

/* Function: al_load_ttf_font_f
 */
ALLEGRO_FONT *al_load_ttf_font_f(ALLEGRO_FILE *file,
    char const *filename, int size, int flags)
{
    FT_Face face;
    FT_ULong unicode;
    FT_UInt g, m = 0;
    ALLEGRO_TTF_FONT_DATA *data;
    ALLEGRO_FONT *f;
    int bytes;
    ALLEGRO_PATH *path;
    FT_Open_Args args;

    data = al_malloc(sizeof *data);
    memset(data, 0, sizeof *data);
    data->stream.read = ftread;
    data->stream.close = ftclose;
    data->stream.pathname.pointer = data;
    data->base_offset = al_ftell(file);
    data->stream.size = al_fsize(file);
    data->file = file;

    memset(&args, 0, sizeof args);
    args.flags = FT_OPEN_STREAM;
    args.stream = &data->stream;

    if (FT_Open_Face(ft, &args, 0, &face) != 0) {
        ALLEGRO_DEBUG("Reading %s failed.\n", filename);
        // Note: Freetype already closed the file for us.
        al_free(data);
        return NULL;
    }

    // FIXME: The below doesn't use Allegro's streaming.
    /* Small hack for Type1 fonts which store kerning information in
     * a separate file - and we try to guess the name of that file.
     */
    path = al_create_path(filename);
    if (!strcmp(al_get_path_extension(path), ".pfa")) {
        const char *helper;
        ALLEGRO_DEBUG("Type1 font assumed for %s.\n", filename);

        al_set_path_extension(path, ".afm");
        helper = al_path_cstr(path, '/');
        FT_Attach_File(face, helper);
        ALLEGRO_DEBUG("Guessed afm file %s.\n", helper);

        al_set_path_extension(path, ".tfm");
        helper = al_path_cstr(path, '/');
        FT_Attach_File(face, helper);
        ALLEGRO_DEBUG("Guessed tfm file %s.\n", helper);
    }
    al_destroy_path(path);

    if (size > 0) {
       FT_Set_Pixel_Sizes(face, 0, size);
    }
    else {
       /* Set the "real dimension" of the font to be the passed size,
        * in pixels.
        */
       FT_Size_RequestRec req;
       req.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
       req.width = 0;
       req.height = (-size) << 6;
       req.horiResolution = 0;
       req.vertResolution = 0;
       FT_Request_Size(face, &req);
    }

    ALLEGRO_DEBUG("Font %s loaded with pixel size %d.\n", filename,
        size);
    ALLEGRO_DEBUG("    ascent=%.1f, descent=%.1f, height=%.1f\n",
        face->size->metrics.ascender / 64.0,
        face->size->metrics.descender / 64.0,
        face->size->metrics.height / 64.0);

    unicode = FT_Get_First_Char(face, &g);
    while (g) {
        unicode = FT_Get_Next_Char(face, unicode, &g);
        if (g > m)
           m = g;
    }

    data->face = face;
    data->flags = flags;
    data->no_premultiply_alpha =
       (al_get_new_bitmap_flags() & ALLEGRO_NO_PREMULTIPLIED_ALPHA);
    data->glyphs_count = m;
    bytes = (m + 1) * sizeof(ALLEGRO_TTF_GLYPH_DATA);
    data->cache = al_malloc(bytes);
    memset(data->cache, 0, bytes);

    if (flags & ALLEGRO_TTF_MONOCHROME) {
       unsigned int i;
       for (i = 0; i < m+1; i++) {
          ((ALLEGRO_TTF_GLYPH_DATA *)(data->cache + i))->monochrome = true;
       }
    }

    _al_vector_init(&data->cache_bitmaps, sizeof(ALLEGRO_BITMAP*));
    ALLEGRO_DEBUG("%s: Preparing cache for %d glyphs.\n", filename, m);

    f = al_malloc(sizeof *f);
    f->height = face->size->metrics.height >> 6;

    f->vtable = &vt;
    f->data = data;

    _al_register_destructor(_al_dtor_list, f,
       (void (*)(void *))al_destroy_font);

    return f;
}


/* Function: al_load_ttf_font
 */
ALLEGRO_FONT *al_load_ttf_font(char const *filename, int size, int flags)
{
   ALLEGRO_FILE *f;
   ALLEGRO_FONT *font;
   ASSERT(filename);

   f = al_fopen(filename, "rb");
   if (!f)
      return NULL;

   /* The file handle is owned by the function and the file is usually only
    * closed when the font is destroyed, in case Freetype has to load data
    * at a later time.
    */
   font = al_load_ttf_font_f(f, filename, size, flags);

   return font;
}

/* Function: al_init_ttf_addon
 */
bool al_init_ttf_addon(void)
{
   if (inited) return false;
   inited = true;

   FT_Init_FreeType(&ft);
   vt.font_height = font_height;
   vt.font_ascent = font_ascent;
   vt.font_descent = font_descent;
   vt.char_length = char_length;
   vt.text_length = text_length;
   vt.render_char = render_char;
   vt.render = render;
   vt.destroy = destroy;
   vt.get_text_dimensions = get_text_dimensions;
    
   al_register_font_loader(".ttf", al_load_ttf_font);
   /* Can't fail right now - in the future we might dynamically load
    * the FreeType DLL here and/or initialize FreeType (which both
    * could fail and would cause a false return).
    */
   return true;
}

/* Function: al_shutdown_ttf_addon
 */
void al_shutdown_ttf_addon(void)
{
   if (!inited) return;
   inited = false;
   
   al_register_font_loader(".ttf", NULL);
   
   FT_Done_FreeType(ft);
}


/* Function: al_get_allegro_ttf_version
 */
uint32_t al_get_allegro_ttf_version(void)
{
   return ALLEGRO_VERSION_INT;
}
