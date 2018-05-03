#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Alo.hpp"

class Drawable
{
public:
	virtual void Render(int x, int y, double zoom) = 0;
};

#endif	//DRAWABLE_H

