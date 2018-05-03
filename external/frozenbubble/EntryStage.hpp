#ifndef ENTRYSTAGE_H
#define ENTRYSTAGE_H

#include "hanna/Stage.hpp"
#include "hanna/Tube3DTransition.hpp" 
#include "hanna/PropBitmap.hpp"
#include "PropWidget.hpp"


class EntryStage : public Stage
{
public:
	EntryStage():music_on(false),sound_on(false),level(1){}
public:
	virtual int OnUpdate();
	virtual bool OnInit();
	virtual void OnRelease();
	virtual bool OnMouseDown(int x, int y);
	virtual bool OnMouseMoving(int x, int y);
	virtual bool OnMouseUp(int x, int y);
    virtual void Render();
    virtual void OnBackground();
    virtual void OnForeground();
private:
    void DisplayLevelOutline(int level);
private:
    enum
    {
        STATE_NORMAL,
        STATE_TRANSITION,
    };
	PropWidget* myMenu;
    PropBitmap* myBubbleLevel;
	Tube3DTransition myTransition;
	int myState;
	int stateTransition;

	bool music_on;
	bool sound_on;
	int  level;
};

#endif	//ENTRYSTAGE_H

