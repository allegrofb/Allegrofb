#ifndef ENTRYSTAGE_H
#define ENTRYSTAGE_H

#include "hanna/Stage.hpp"
#include "hanna/PropWidget.hpp"
#include "hanna/ResourceManager.hpp"
#include "hanna/PropFont.hpp"

class EntryStage : public Stage
{
public:
	EntryStage():music_on(false),sound_on(false),level(1){}
public:
	virtual int OnUpdate();
	virtual bool OnInit();
	virtual void OnRelease();
	virtual void OnChar(int keycode, int unichar);
	virtual bool OnMouseDown(int x, int y);
	virtual bool OnMouseMoving(int x, int y);
	virtual bool OnMouseUp(int x, int y);
    virtual void OnBackGround();
    virtual void OnForeGround();
    virtual void OnOrientation(int orientation);
    
private:
    void DisplayLevelOutline(int level);
    
private:
	PropWidget* myMenu;
    PropBitmap* myBubbleLevel;

	bool music_on;
	bool sound_on;
	int  level;
};


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

#endif	//ENTRYSTAGE_H

