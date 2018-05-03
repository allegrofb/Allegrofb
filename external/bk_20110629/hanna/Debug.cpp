#include "Alo.hpp"
#include "Debug.hpp"
#include <stdarg.h>
#include <stdio.h>

void debug_message(const char * fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   vprintf(fmt, ap);
   va_end(ap);
}

/*
 * Return the path to user resources (save states, configuration)
 */

#ifdef ALLEGRO_MSVC
#define snprintf _snprintf
#endif

const char* util_get_path(const char* fmt, ...)
{
    va_list ap;
    static char res[512];
    static ALLEGRO_PATH *dir;
    static ALLEGRO_PATH *path;
    
    va_start(ap, fmt);
    memset(res, 0, 512);
    snprintf(res, 511, fmt, ap);
    
    if (!dir) {
        dir = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
#ifdef ALLEGRO_MSVC
        {
            /* Hack to cope automatically with MSVC workspaces. */
            const char *last = al_get_path_component(dir, -1);
            if (0 == strcmp(last, "Debug")
                || 0 == strcmp(last, "RelWithDebInfo")
                || 0 == strcmp(last, "Release")
                || 0 == strcmp(last, "Profile")) {
                al_remove_path_component(dir, -1);
            }
        }
#endif
        al_append_path_component(dir, "data");
    }
    
    if (path)
        al_destroy_path(path);
    
    path = al_create_path(res);
    al_rebase_path(dir, path);
    return al_path_cstr(path, '/');
}


