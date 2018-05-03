#ifndef PROPBASE_H
#define PROPBASE_H

#include "Bitmap.hpp"
#include "Drawable.hpp"


class PropBase : public Drawable
{
public:
	PropBase(int x,int y,int h,int w, bool v = true)
		:myX(x),myY(y),myHeight(h),myWidth(w),myVisible(v),myDelay(0)
	{}
public:
	void SetVisible(bool visible, int delay=0){	myDelay = delay;myVisible = visible;};
	virtual void Render(int offsetx,int offsety, double zoom){};
public:
	int myX;
	int myY;
	int myHeight;
	int myWidth;
	bool myVisible;
	int myDelay;
};


struct Function
{
	void operator()(int x,int y,int h, int w){};
};

template<class T=Function>
class PropDrawing : public PropBase
{
public:
	PropDrawing(int x,int y,int h,int w, bool v = true)
		:PropBase(x,y,h,w,v)
	{}
public:
	virtual void Render(int offsetx,int offsety, double zoom)
	{
		if(myVisible)
		RenderFunction(myX+offsetx,myY+offsety,myHeight*zoom,myWidth*zoom);
	};
	T RenderFunction;
};


#endif	//PROPBASE_H

