#ifndef PROP_H
#define PROP_H

#include "Bitmap.hpp"
#include "PropBase.hpp"

class Prop : public PropBase
{
public:
	Prop(Bitmap *p,int x,int y,int h,int w, bool v = true)
		:bitmap(p),PropBase(x,y,h,w,v)
	{}
public:
	virtual void Render(int offsetx,int offsety, double zoom);
private:
	Bitmap* bitmap;
};



class PropBitmap : public PropBase
{
public:
	PropBitmap(int x,int y,int h,int w, bool v = true)
    :PropBase(x,y,h,w,v),bitmap(NULL)
	{}
    ~PropBitmap()
    {
        if(bitmap)
            delete bitmap;
    }
public:
	virtual void Render(int offsetx,int offsety, double zoom);
    void Update(Drawable* p,int offsetx,int offsety, double zoom);
private:
    ALLEGRO_BITMAP* GetBitmap(int w, int h);
private:
	Bitmap *bitmap;
};


#endif	//PROP_H

