#include "PropRotation.hpp"

void PropRotation::Render(int offsetx,int offsety,double zoom)
{
	if(myVisible)
		bitmap->Draw(myCx,myCy,myX+offsetx,myY+offsety,myAngle,zoom);
}

void PropRotation::SetRotation(float angle)
{
	myAngle = angle;
}

