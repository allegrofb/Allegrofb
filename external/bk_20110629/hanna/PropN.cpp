#include "PropN.hpp"

void PropN::Render(int offsetx,int offsety, double zoom)
{
	if(myVisible)
	{
		if(myIndex >= myData.len)
		{
			myIndex = 0;
			if(myRunningMode != myDefaultMode)
			{
				myRunningMode = myDefaultMode;
				SetMode(myDefaultMode);
			}
		}

		if(myData.len > 0)
		{
			bitmap->Draw(myX+offsetx,myY+offsety,myData.mode[myIndex],zoom);

			if(al_get_time()-myTime > 0.25)
			{
				myTime = al_get_time();
				myIndex++;
			}
		}
	}
}

void PropN::AddMode(const int mode[], int len)
{
	Data data;
	if(len > 0)
	{
		data.len = len;
		data.mode = new int[len];

		for(int i = 0;i<len;i++)
		{
			data.mode[i] = mode[i];
		}
		myMode.push_back(data);
	}
}

void PropN::SetMode(int mode)
{
	if(mode > 0 && mode <= (int)myMode.size())
	{
		myData	  = myMode[mode-1];
		myDefaultMode = mode;
	}
	else
	{
		myData.len = 0;
	}

	myIndex = 0;
}

void PropN::RunOneTime(int mode)
{
	int tmp = myDefaultMode;
	myRunningMode = mode;
	SetMode(mode);
	myDefaultMode = tmp;
}
