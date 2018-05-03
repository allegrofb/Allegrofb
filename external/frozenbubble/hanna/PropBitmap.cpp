#include "PropBitmap.hpp"

ALLEGRO_BITMAP* PropBitmap::GetBitmap(int w, int h)
{
    ALLEGRO_BITMAP* p = NULL;
    
    if(bitmap == NULL)
    {
        p = al_create_bitmap(w,h);
        if(p == NULL)
            return NULL;
        
        bitmap = new Bitmap(p);
    }
    
    return bitmap->GetBitmap();
}

void PropBitmap::Update(Drawable* p,int offsetx,int offsety, double zoom)
{
    ALLEGRO_BITMAP *target = GetBitmap(myWidth,myHeight);
    
    if(target == NULL)
        return;
    
    al_set_target_bitmap(target);
    al_clear_to_color(al_map_rgba_f(0, 0, 0, 125));
    
    p->Render(offsetx,offsety,zoom);
    
    al_set_target_backbuffer(al_get_current_display());
    
    return;
    
}

void PropBitmap::Render(int offsetx,int offsety, double zoom)
{
    if(bitmap == NULL)
        return;
    
	if(myDelay)
	{
		myDelay--;
        if(myDelay <= 0)
            myVisible = false;
	}
    
	if(myVisible)
	{
        int op,src,dst;        
        al_get_blender(&op,&src,&dst);
        
        al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_ALPHA);
        
        bitmap->Draw(myX+offsetx,myY+offsety,zoom);        
        
        al_set_blender(op,src,dst);        
	}
}

