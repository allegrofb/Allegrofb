#ifndef SPLASHSTAGE_H
#define SPLASHSTAGE_H

#include "hanna/Stage.hpp"
#include "hanna/ResourceManager.hpp"
#include "hanna/PropFont.hpp"

class SplashStage : public Stage
{
public:
	SplashStage(AsynLoader* p){myLoader=p;};
public:
	virtual int OnUpdate();
	virtual bool OnInit();
	virtual void OnRelease();
private:
	PropFont*	myLevelStr;
	Prop*		mySplash;
	AsynLoader* myLoader;
};

#endif	//SPLASHSTAGE_H

