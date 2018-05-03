#include "BitmapFont.hpp"

BitmapFont* BitmapFont::LoadFrom(const char* filename,const int* pos,const char* chars, int height)
{
	Bitmap* ptr = Bitmap::LoadFrom(filename);
	if(!ptr)
	{
		return NULL;
	}
	return new BitmapFont(ptr,pos,chars,height);
}

BitmapFont::BitmapFont(Bitmap *p,const int* pos, const char* chars, int height)
	: Bitmap(p),position(pos),characters(chars),charsHeight(height)
{
	charsMaximum = strlen(characters);
}


void BitmapFont::Draw(int x, int y, const char* str, double zoom)
{
	int len = strlen(str);
	int width = 0;
	int height = charsHeight;
	int pos_x = 0;
	for(int i = 0;i<len;i++)
	{
		if(str[i] == 0x0A)
		{
			y += height;
			pos_x = 0;
			continue;
		}
		else if(str[i] == ' ')
		{
			pos_x += width;
			continue;
		}
		volatile int j = str[i]-characters[0];
		if(j > charsMaximum)
		{
			j = 0;
		}
		width = position[j+1]-position[j];
		Bitmap::Draw(position[j],0,width,height,x+pos_x,y,zoom);
		pos_x += width;
	}
}

void BitmapFont::GetWidthHeight(const char* str, int& width,int& height)
{
	int y = charsHeight;
	int len = strlen(str);
	int pos_x = 0;
	width = 0;
	height = charsHeight;
	for(int i = 0;i<len;i++)
	{
		if(str[i] == 0x0A)
		{
			y += height;
			pos_x = 0;
			continue;
		}
		else if(str[i] == ' ')
		{
			pos_x += width;
			continue;
		}
		volatile int j = str[i]-characters[0];
		if(j > charsMaximum)
		{
			j = 0;
		}
		width = position[j+1]-position[j];
		//Bitmap::Draw(position[j],0,width,height,x+pos_x,y);
		pos_x += width;
	}

	width = pos_x;
	return;
}
