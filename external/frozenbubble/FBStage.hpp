#ifndef FBSTAGE_H
#define FBSTAGE_H

#include "hanna/Stage.hpp"
#include "hanna/BitmapArray.hpp"
#include "hanna/PropRotation.hpp"
#include "hanna/PropN.hpp"
#include "hanna/PropFont.hpp"
#include "hanna/Tube3DTransition.hpp"
#include "PropWidget.hpp"
#include "FBBubble.hpp"
#include "FBCompressor.hpp"

class FBStage : public Stage
{
public:
	FBStage(){}
public:
	virtual int  OnUpdate();
	virtual bool OnInit();
	virtual void OnRelease();
	virtual void OnChar(int keycode, int unichar);
	virtual bool OnMouseDown(int x, int y);
	virtual bool OnMouseMoving(int x, int y);
	virtual bool OnMouseUp(int x, int y);
	virtual void OnShakeGesture();
	virtual void Render();
private:
	int  currentLevel;
	bool musicOn;
	bool soundOn;
	void Init(int level);
	void UpdateMenu();
    void UpdateCenterOffset(int &x, int &y);
private:
    enum
    {
        STATE_UNDEFINED ,		
        STATE_NORMAL	,	
        STATE_WIN		,	
        STATE_LOSE		,	
        STATE_MENU_PRE	,	
        STATE_MENU		,	
        STATE_TRANSITION,	
    };
	void StateNormal();
	void StateWin();
	void StateLose();
	void StateMenu();
	int  myState;
private:
	PropRotation* myLauncher;
	PropN*		  myPenguins;
	FBBubble*	  myBubble;
	PropFont*	  myLevelStr;
	Prop*		  myWinPanel;
	Prop*		  myLosePanel;
	Prop*		  myHurryPanel;
	FBCompressor* myCompressor;
	int           hurryTime;
	float         angle;
	int			  fixedBubbles;
	int			  endOfGame;
	int			  blinkDelay;

	PropWidget*   myMenu;
	PropBase*     myTransparentBackground;
	int			  myMenuStateBk;

	Tube3DTransition myTransition;
	int           stateTransition;
};

#endif	//FBSTAGE_H

