#include "BitmapN.hpp"

BitmapN* BitmapN::LoadFrom(const char* filename, int w, int h, int row, int col)
{
	Bitmap*p = Bitmap::LoadFrom(filename);

	if(p)
	{
		return new BitmapN(p,w,h,row,col);
	}
	return NULL;
}

BitmapN::BitmapN(Bitmap* p,int w, int h, int row,int col)
	: Bitmap(p)
{
	myData = new Data[row*col];

	for(int i = 0;i<row;i++)
	for(int j = 0;j<col;j++)
	{
		myData[j+i*col].h = h/row;
		myData[j+i*col].w = w/col;
		myData[j+i*col].l = j*w/col;
		myData[j+i*col].t = i*h/row;
	}

	myCol = col;
	myRow = row;
}

//void BitmapN::Draw(int x,int y,int row, int col)
//{
//	int sx = myData[row-1+(col-1)*myRow].l;
//	int sy = myData[row-1+(col-1)*myRow].t;
//	int sw = myData[row-1+(col-1)*myRow].w;
//	int sh = myData[row-1+(col-1)*myRow].h;
//	Bitmap::Draw(sx,sy,sw,sh,x,y);
//}

void BitmapN::Draw(int x,int y,int n, double zoom)
{
	int sx = myData[n].l;
	int sy = myData[n].t;
	int sw = myData[n].w;
	int sh = myData[n].h;
	Bitmap::Draw(sx,sy,sw,sh,x,y,zoom);
}
