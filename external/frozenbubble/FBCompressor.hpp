#ifndef FBCOMPRESSOR_H
#define FBCOMPRESSOR_H

#include "hanna/PropBase.hpp"

class FBCompressor : public PropBase
{
public:
	FBCompressor(Bitmap *c,Bitmap* b,int x,int y,int h,int w, bool v = true)
		:PropBase(x,y,h,w,v),compressor(c),body(b),pos(0)
	{}
	virtual void Render(int offsetx,int offsety,double zoom);
public:
	void MoveDown(){pos++;};
	void Reset(){pos=0;};
private:
	Bitmap* compressor;
	Bitmap* body;
	int pos;
};


#endif	//FBCOMPRESSOR_H

