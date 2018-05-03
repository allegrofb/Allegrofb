#ifndef BITMAPARRAY_HPP
#define BITMAPARRAY_HPP

#include "Alo.hpp"
#include "Bitmap.hpp"
#include <vector>

class BitmapArray
{
public:
	static BitmapArray* LoadFrom(const char* filenames[]);
public:
	Bitmap* Get(int i);
	BitmapArray(std::vector<Bitmap*>const &bitmaps);
	~BitmapArray();
protected:
	std::vector<Bitmap*> bitmapVector;
	int maxIndex;
};

#endif // BITMAPARRAY_HPP

