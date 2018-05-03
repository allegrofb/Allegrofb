#ifndef BITMAP_HPP
#define BITMAP_HPP

#include "Alo.hpp"

//hyjiang, only hold resource and in charge of calling allegro function
class Bitmap	
{
public:
	static Bitmap* LoadFrom(const char* filename);//pcx,bmp,tga//
public:
	void Draw();
	void Draw(int x, int y);
	void Draw(int sx,int sy,int sw,int sh,int x,int y);
	void Draw(int cx,int cy,int x,int y,float angle);
	void Draw(int x, int y, double zoom);
	void Draw(int sx,int sy,int sw,int sh,int x,int y, double zoom);
	void Draw(int cx,int cy,int x,int y,float angle, double zoom);

public:	
	Bitmap(ALLEGRO_BITMAP *p);
	Bitmap(Bitmap *p);
	~Bitmap();
    ALLEGRO_BITMAP* GetBitmap(){return bitmap;}
private:
	ALLEGRO_BITMAP *bitmap;
	int height;
	int width;
};

#endif // BITMAP_HPP

