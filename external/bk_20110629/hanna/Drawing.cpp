#include "Drawing.hpp"


void Drawing::Draw(Resource* p, int x, int y)
{
	if(dynamic_cast<Bitmap*>(p))
	{
		(Bitmap*)p->Draw(x,y);
	}

	
	
	return;
}



