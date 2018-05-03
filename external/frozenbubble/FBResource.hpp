#ifndef FBRESOURCE_HPP
#define FBRESOURCE_HPP

#include "hanna/ResourceManager.hpp"
#include "hanna/Bitmap.hpp"
#include "hanna/BitmapN.hpp"
#include "hanna/BitmapArray.hpp"
#include "hanna/BitmapFont.hpp"
#include "hanna/Singleton.hpp"
#include "hanna/Prop.hpp"
#include "hanna/PropN.hpp"
#include "hanna/PropRotation.hpp"
#include "hanna/Sample.hpp"
#include "hanna/Stream.hpp"
#include "hanna/PropFont.hpp"
#include "PropWidget.hpp"
#include "FBBubble.hpp"
#include "FBLevels.hpp"
#include "FBCompressor.hpp"

#define NEWROOT_SOLO	0
#define LOSE			1
#define NOH				2
#define REBOUND			3
#define STICK			4
#define DESTROY_GROUP	5
#define APPLAUSE		6
#define HURRY			7
#define LAUNCH			8

#define INTROZIK		0
#define MAINZIK			1

#define SEQUENCE_VOID		1 
#define SEQUENCE_TURN_LEFT	2
#define SEQUENCE_TURN_RIGHT	3	
#define SEQUENCE_FIRE		4
#define SEQUENCE_LOST		5
#define SEQUENCE_WON		6
#define SEQUENCE_FULL		7

#define MENU_ID_RESUME		1 
#define MENU_ID_START		1 
#define MENU_ID_LEVEL		2
#define MENU_ID_MUSIC		3
#define MENU_ID_SOUND		4
#define MENU_ID_EXIT		5

#define STAGE_SPLASH	0
#define STAGE_ENTRY		1
#define STAGE_GAME		2


class FBResource_ : public ResourceManager
{
private:
	BitmapArray* pBubble;
	BitmapArray* pBubbleColorBlind;
	BitmapArray* pBubbleFrozen;
	BitmapArray* pFixed;
	Bitmap* pBackGround;
	Bitmap* pAppFrozenBubble;
	Bitmap* pBubbleBlink;

	BitmapFont* pFont;
	Bitmap* pCloseEyes;
	Bitmap* pCompressor;
	Bitmap* pCompressorBody;
	Bitmap* pHurry;
	Bitmap* pLauncher;
	Bitmap* pLife;
	Bitmap* pLosePanel;
	BitmapN* pPenguins;
	Bitmap* pSplash;
	Bitmap* pVoidPanel;
	Bitmap* pWinPanel;

public:
	virtual void Destroy(void);
	virtual bool Load(void);
public:
	Prop* CreateBackGround();
	FBCompressor* CreateCompressor();
	FBBubble* CreateFBBubble();
	PropRotation* CreateLauncher();
	PropN* CreatePenguins();
	Prop* CreateLosePanel();
	Prop* CreateWinPanel();
	Prop* CreateHurryPanel();
	FBLevels* Levels;
	PropFont* CreateFont();
	PropFont* DebugMessage;
	Prop* CreateSplash();
	PropWidget* CreateEntryMenu(int levelnum, bool music_on, bool sound_on);
	PropBase* CreateTransparentBackground();
	PropWidget* CreateMainMenu(int levelnum, bool music_on, bool sound_on);
    PropTextAnimation* CreateTextAnimation();
    PropBase* CreateTransparentBackground4MainMenu();
	int NextBubble();
	int JumpingX();
	int JumpingY();
	int FallingY();

	BitmapFont* GetFont(){return pFont;}
    Bitmap*     GetOther(){return pVoidPanel;}
};

typedef singleton_default<FBResource_> FBResource;

extern const char RES_CHARACTERS[];
extern const int RES_POSITION[];
extern const char* RES_BUBBLE_FONT;
extern const char* RES_VOID_PANEL;
extern const int FONT_HEIGHT;
extern const char* RES_APP_FROZEN_BUBBLE;


#endif // FBRESOURCE_HPP

