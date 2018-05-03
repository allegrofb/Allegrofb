#include "Bitmap.hpp"

Bitmap::Bitmap(ALLEGRO_BITMAP *p)
{
	bitmap = p;
	height = al_get_bitmap_height(p);
	width = al_get_bitmap_width(p);
}

Bitmap::Bitmap(Bitmap *p)
{
	bitmap = p->bitmap;
	height = p->height;
	width = p->width;
}

Bitmap::~Bitmap()
{
	if (!bitmap)
		return;
      
	al_destroy_bitmap(bitmap);
	bitmap = 0;
}

Bitmap* Bitmap::LoadFrom(const char* filename)
{
	ALLEGRO_BITMAP* bitmap = NULL;
	ALLEGRO_BITMAP* membitmap = NULL;
    
    //load memeory, then load video, does it need ??
   	al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);
   	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   	membitmap = al_load_bitmap(util_get_path(filename));
   	if (!membitmap)
   	{
		debug_message("Error loading bitmap %s\n", filename);
      	return NULL;
  	}
    
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    
    // FIXME: 
    // Now try to split the memory bitmap into display bitmaps?
    bitmap = al_clone_bitmap(membitmap);
    if (!bitmap)
        bitmap = membitmap;

   	return new Bitmap(bitmap);
}

void Bitmap::Draw(int x, int y)
{
	al_draw_bitmap(bitmap,x,y,0);
}

void Bitmap::Draw()
{
	al_draw_bitmap(bitmap,0,0,0);
}

void Bitmap::Draw(int sx,int sy,int sw,int sh,int x,int y)
{
	al_draw_bitmap_region(bitmap,sx,sy,sw,sh,x,y,0);
}

void Bitmap::Draw(int cx,int cy,int x,int y,float angle)
{
	al_draw_rotated_bitmap(bitmap,cx,cy,x,y,-angle,0);
}

void Bitmap::Draw(int x, int y, double zoom)
{
	al_draw_scaled_bitmap(bitmap,0,0,width,height,x,y,width*zoom,height*zoom,0);
}

void Bitmap::Draw(int sx,int sy,int sw,int sh,int x,int y, double zoom)
{
	al_draw_scaled_bitmap(bitmap,sx,sy,sw,sh,x,y,sw*zoom,sh*zoom,0);
}

void Bitmap::Draw(int cx,int cy,int x,int y,float angle, double zoom)
{
	al_draw_scaled_rotated_bitmap(bitmap,cx,cy,x,y,zoom,zoom,-angle,0);
}
