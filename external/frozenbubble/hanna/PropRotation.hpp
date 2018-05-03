#ifndef PROPROTATION_H
#define PROPROTATION_H

#include "Bitmap.hpp"
#include "PropBase.hpp"

#define ANGLE(x) (x*ALLEGRO_PI/180)

class PropRotation : public PropBase
{
public:
	PropRotation(Bitmap *p,int cx,int cy,int x,int y,int h,int w, bool v = true)
		:PropBase(x,y,h,w,v),myCx(cx),myCy(cy),myAngle(0),bitmap(p)
	{}
public:
	void SetRotation(float angle);
	virtual void Render(int offsetx,int offsety,double zoom);
private:
	int myCx;
	int myCy;
	float myAngle;
	Bitmap* bitmap;
};


#endif	//PROPROTATION_H

