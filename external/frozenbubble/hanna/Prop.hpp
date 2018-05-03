#ifndef PROP_H
#define PROP_H

#include "Bitmap.hpp"
#include "PropBase.hpp"

class Prop : public PropBase
{
public:
	Prop(Bitmap *p,int x,int y,int h,int w, bool v = true)
		:PropBase(x,y,h,w,v),bitmap(p)
	{}
public:
	virtual void Render(int offsetx,int offsety, double zoom);
private:
	Bitmap* bitmap;
};

#endif	//PROP_H

