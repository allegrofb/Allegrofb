#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "hanna/PropBase.hpp"

class Compressor : public PropBase
{
public:
	Compressor(Bitmap *c,Bitmap* b,int x,int y,int h,int w, bool v = true)
		:compressor(c),body(b),PropBase(x,y,h,w,v),pos(0)
	{}
public:
	virtual void Render(int offsetx,int offsety,double zoom);
	void MoveDown(){pos++;};
	void Reset(){pos=0;};
private:
	Bitmap* compressor;
	Bitmap* body;
	int pos;
};


#endif	//COMPRESSOR_H

