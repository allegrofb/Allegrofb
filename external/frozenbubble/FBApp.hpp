#ifndef FBAPP_H
#define FBAPP_H

#include "hanna/App.hpp"

class FBApp : public App
{
public:
	virtual bool OnInit();
	virtual void OnRelease();
};


#endif	//FBAPP_H

