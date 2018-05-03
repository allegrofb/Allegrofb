#include "Compressor.hpp"

void Compressor::Render(int offsetx,int offsety,double zoom)
{
	if(this->myVisible)
	{
		for (int i = 0; i < pos; i++)
		{
			body->Draw(235+offsetx,28*i-4+offsety);
			body->Draw(391+offsetx,28*i-4+offsety);
		}
		compressor->Draw(160+offsetx,-7+28*pos+offsety);
	}
}
