#ifndef BITMAPN_H
#define BITMAPN_H

#include "Bitmap.hpp"

class BitmapN : public Bitmap
{
public:	
	static BitmapN* LoadFrom(const char* filename, int w, int h, int row, int col);
	BitmapN(Bitmap* p,int w, int h, int row,int col);
	void Draw(int x,int y,int n, double zoom);
private:
	int myRow;
	int myCol;
	struct Data
	{
		int l;
		int t;
		int w;
		int h;
	};
	Data* myData;
};

#endif	//BITMAPN_H

