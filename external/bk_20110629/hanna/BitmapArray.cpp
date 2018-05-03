#include "BitmapArray.hpp"

BitmapArray::BitmapArray(std::vector<Bitmap*>const &bitmaps)
{
	bitmapVector.assign(bitmaps.begin(),bitmaps.end());
	maxIndex = bitmapVector.size();
}

BitmapArray::~BitmapArray()
{
	for(std::vector<Bitmap*>::iterator i = bitmapVector.begin();
		i != bitmapVector.end();i++)
	{
		delete *i;		
	}
	bitmapVector.clear();		
}

BitmapArray* BitmapArray::LoadFrom(const char* filenames[])
{
	std::vector<Bitmap*> v;
		
	for(int i = 0;filenames[i];i++)
	{
		if(Bitmap* ptr = Bitmap::LoadFrom(filenames[i]))
		{
			v.push_back(ptr);
		}
	}
	if(v.size())
	{
		return new BitmapArray(v);
	}
	
	return NULL;
}

Bitmap* BitmapArray::Get(int i)
{
	if(i < 0 && i >= maxIndex)
	{
		return NULL;
	}		

	return bitmapVector[i];
}
//
//void BitmapArray::Add(const_iterator begin,const_iterator end)
//{
//	for(const_iterator i = begin;i != end;i++)
//	{
//		bitmapVector.push_back(*i);
//	}
//}

