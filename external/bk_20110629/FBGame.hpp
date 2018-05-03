#ifndef FBGAME_H
#define FBGAME_H

#include "FBBubble.hpp"
#include <list>

class FBGame
{
public:

	virtual void Init();
	virtual void Run();
	virtual void Release();
public:
	Bitmap* pBackGround;



	void Add(FBBubble* p);
	void Remove(FBBubble* p);
	void Render();
private:
	FBBubble* fixedList[8][10];
	std::list<FBBubble*> bubbleList;
};

#endif	//FBGAME_H

