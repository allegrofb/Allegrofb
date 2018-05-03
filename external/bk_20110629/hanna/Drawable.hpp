#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Alo.hpp"

class Drawable
{
public:
	virtual void Render(int x, int y, double zoom) = 0;
    virtual void RenderTo(ALLEGRO_BITMAP* target, int x, int y, double zoom)
    {
        Render(x,y,zoom);
    }
};


#endif	//DRAWABLE_H

