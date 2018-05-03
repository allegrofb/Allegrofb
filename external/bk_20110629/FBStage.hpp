#ifndef FBSTAGE_H
#define FBSTAGE_H

#include "hanna/Stage.hpp"
#include "hanna/BitmapArray.hpp"
#include "hanna/PropRotation.hpp"
#include "hanna/PropN.hpp"
#include "hanna/PropFont.hpp"
#include "hanna/PropWidget.hpp"
#include "FBBubble.hpp"
#include "Compressor.hpp"

#define STATE_UNDEFINED	0
#define STATE_NORMAL	1
#define STATE_WIN		2
#define STATE_LOSE		3
#define STATE_MENU_PRE	4
#define STATE_MENU		5

class FBStage : public Stage
{
public:
	virtual int  OnUpdate();
	virtual bool OnInit();
	virtual void OnRelease();
	virtual void OnAction();
	virtual void OnChar(int keycode, int unichar);
	virtual bool OnMouseDown(int x, int y);
	virtual bool OnMouseMoving(int x, int y);
	virtual bool OnMouseUp(int x, int y);
	virtual void OnShakeGesture();
public:
	FBStage():currentLevel(1),myMenuStateBk(STATE_UNDEFINED),myMenuFlag(false)
	{}
private:
	int  currentLevel;
	bool musicOn;
	bool soundOn;
	void Init(int level);
	void UpdateMenu();
private:
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
	Compressor*	  myCompressor;
	int           hurryTime;
	float         angle;
	int			  fixedBubbles;
	int			  endOfGame;
	int			  blinkDelay;

	PropWidget*   myMenu;
	PropBase*     myTransparentBackground;
	int			  myMenuStateBk;
	bool		  myMenuFlag;
};

#endif	//FBSTAGE_H

