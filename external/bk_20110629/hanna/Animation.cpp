#include "Animation.hpp"

Animation::Animation(std::vector<Bitmap*>const &bitmaps)
	:BitmapArray(bitmaps)
{
	//bitmapVector.assign(bitmaps.begin(),bitmaps.end());
	currentIndex = 0;
	maxIndex = bitmapVector.size();
}

Animation::~Animation()
{
	//for(std::vector<Bitmap*>::iterator i = bitmapVector.begin();
	//	i != bitmapVector.end();i++)
	//{
	//	delete *i;		
	//}
	//bitmapVector.clear();		
}

Animation* Animation::LoadFrom(const char* filenames[])
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
		return new Animation(v);
	}
	
	return NULL;
}

void Animation::Start()
{
	currentIndex = 0;
}

void Animation::Stop()
{
	currentIndex = maxIndex;
}

void Animation::Render()
{
	if(currentIndex < maxIndex)
	{
		bitmapVector[currentIndex]->Draw();
		currentIndex++;
	}
}
