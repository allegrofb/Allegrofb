#ifndef PROPFONT_H
#define PROPFONT_H

#include "BitmapFont.hpp"
#include "PropBase.hpp"

class PropFont : public PropBase    //hyjiang, modify name to PropText
{
public:
	PropFont(BitmapFont* p,int x,int y,int h,int w, bool v = true)
		: PropBase(x,y,h,w,v),bitmapFont(p)
	{
		memset(Str,0, sizeof(Str));
	}
public:
	virtual void Render(int offsetx,int offsety, double zoom);
	void Assign(int x);
	void Assign(int x ,int y);
	void Assign(const char* str);
private:
	char Str[80];
	BitmapFont* bitmapFont;
};


class PropTextAnimation : public PropBase
{
public:
	PropTextAnimation(BitmapFont* p, int x,int y,int h,int w, const char* str = NULL, bool v = true)
    : PropBase(x,y,h,w,v),bitmapFont(p)
	{
		memset(Str,0, sizeof(Str));
        myStrHeight = 0;
        myStrWidth = 0;
        myAnimationStep = 3;
        if(str)
        {
            strncpy(Str,str,128);
        }
	}
public:
	virtual void Render(int offsetx,int offsety, double zoom);
	void Assign(const char* str);
private:
	char Str[128];
	BitmapFont* bitmapFont;
    int myStrWidth;
    int myStrHeight;
    int myAnimationStep;
    int myStrX;
    int myStrY;
};

#endif	//PROPFONT_H

