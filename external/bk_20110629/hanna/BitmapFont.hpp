#ifndef BITMAPFONT_HPP
#define BITMAPFONT_HPP

#include "Alo.hpp"
#include "Bitmap.hpp"

class BitmapFont : public Bitmap
{
public:	
	static BitmapFont* LoadFrom(const char* filename,const int* pos,const char* chars, int height);
	BitmapFont(Bitmap *p,const int* pos, const char* chars, int height);
	void Draw(int x, int y, const char* str, double zoom);		//capital character
	void GetWidthHeight(const char* str, int& width,int& height);
private:
	const int* position;
	const char* characters;
	int charsMaximum;
	int charsHeight;
};

#endif // BITMAPFONT_HPP

