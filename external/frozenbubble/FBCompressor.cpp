#include "FBCompressor.hpp"

void FBCompressor::Render(int offsetx,int offsety,double zoom)
{
	if(this->myVisible)
	{
		for (int i = 0; i < pos; i++)
		{
			body->Draw(235+offsetx,28*i-4+offsety,zoom);
			body->Draw(391+offsetx,28*i-4+offsety,zoom);
		}
		compressor->Draw(160+offsetx,-7+28*pos+offsety,zoom);
	}
}
