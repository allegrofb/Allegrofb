#include "PropFont.hpp"
#include <stdio.h>

void PropFont::Render(int offsetx,int offsety, double zoom)
{
	if(myVisible)
	{
		bitmapFont->Draw(myX+offsetx,myY+offsety,Str,zoom);
	}
}

void PropFont::Assign(int x)
{
	char tmp[80] = {0};
//	itoa(x,tmp,10);
    sprintf(tmp,"%d",x);
	strcpy(Str,tmp);
}

void PropFont::Assign(int x ,int y)
{
	char tmp[80] = {0};
	char tmp2[80] = {0};
    sprintf(tmp,"%d",x);
    sprintf(tmp2,"%d",y);
//	itoa(x,tmp,10);
//	itoa(y,tmp2,10);
	strcpy(Str,tmp);
	strcat(Str,"\n");
	strcat(Str,tmp2);
}

void PropFont::Assign(const char* str)
{
	strncpy(Str,str,80);
}
