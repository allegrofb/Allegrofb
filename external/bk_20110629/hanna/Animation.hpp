#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "Alo.hpp"
#include "BitmapArray.hpp"
#include <vector>

class Animation : public BitmapArray
{
public:
	static Animation* LoadFrom(const char* filenames[]);
public:
	void Start();
	void Stop();
	void Render();
public:	
	Animation(std::vector<Bitmap*>const &bitmaps);
	~Animation();
private:
	//std::vector<Bitmap*> bitmapVector;
	int currentIndex;
	int maxIndex;
};

#endif // ANIMATION_HPP

