#ifndef PROPFONT_H
#define PROPFONT_H

#include "BitmapFont.hpp"
#include "PropBase.hpp"

class PropFont : public PropBase
{
public:
	PropFont(BitmapFont* p,int x,int y,int h,int w, bool v = true)
		: PropBase(x,y,h,w,v),bitmapFont(p)
	{
		memset(Str,0, sizeof(Str));
	}
public:
	virtual void Render(int offsetx,int offsety, double zoom);
	char Str[80];
	void Assign(int x);
	void Assign(int x ,int y);
	void Assign(const char* str);
private:
	BitmapFont* bitmapFont;
};


#endif	//PROPFONT_H

