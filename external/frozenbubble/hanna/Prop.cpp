#include "Prop.hpp"

void Prop::Render(int offsetx,int offsety, double zoom)
{
	if(myDelay)
	{
		myDelay--;
		return;
	}

	if(myVisible)
	{
		bitmap->Draw(myX+offsetx,myY+offsety,zoom);
	}
}

