#ifndef PROPN_H
#define PROPN_H

#include "BitmapN.hpp"
#include "PropBase.hpp"
#include <vector>

class PropN : public PropBase
{
public:
	PropN(BitmapN *p,int x,int y,int h,int w, bool v = true)
		:bitmap(p),PropBase(x,y,h,w,v),myIndex(0),myTime(0),
		myDefaultMode(0),myRunningMode(0)
	{
	}
	~PropN()
	{
		for(std::vector<Data>::const_iterator i = myMode.begin();
			i != myMode.end();i++)
		{
			if(i->mode)
				delete[] i->mode;
		}
	}
public:
	void AddMode(const int mode[], int len);	
	void SetMode(int mode);	
	void RunOneTime(int mode);
	virtual void Render(int offsetx,int offsety,double zoom);
private:
	struct Data
	{
	public:
		Data():mode(NULL),len(0){}
		int* mode;
		int len;
	};
	BitmapN* bitmap;

	std::vector<Data> myMode;
	Data myData;
	int myIndex;
	double myTime;
	int myDefaultMode;
	int myRunningMode;
};


#endif	//PROPN_H

