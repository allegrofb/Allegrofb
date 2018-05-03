#include "FBResource.hpp"
#include "hanna/Sample.hpp"


const char* RES_LEVELS = "res/levels.txt";

const char* RES_BUBBLE[] = {
    "res/bubble_1.gif",
    "res/bubble_2.gif",
    "res/bubble_3.gif",
    "res/bubble_4.gif",
    "res/bubble_5.gif",
    "res/bubble_6.gif",
    "res/bubble_7.gif",
    "res/bubble_8.gif",
	0
};

const char* RES_BUBBLE2[] = {
    "res/bubble_colourblind_1.gif",
    "res/bubble_colourblind_2.gif",
    "res/bubble_colourblind_3.gif",
    "res/bubble_colourblind_4.gif",
    "res/bubble_colourblind_5.gif",
    "res/bubble_colourblind_6.gif",
    "res/bubble_colourblind_7.gif",
    "res/bubble_colourblind_8.gif",
	0
};

const char* RES_FROZEN[] = {
    "res/frozen_1.gif",
    "res/frozen_2.gif",
    "res/frozen_3.gif",
    "res/frozen_4.gif",
    "res/frozen_5.gif",
    "res/frozen_6.gif",
    "res/frozen_7.gif",
    "res/frozen_8.gif",
	0
};

const char* RES_FIXED[] = {
    "res/fixed_1.gif",
    "res/fixed_2.gif",
    "res/fixed_3.gif",
    "res/fixed_4.gif",
    "res/fixed_5.gif",
    "res/fixed_6.gif",
	0
};

const char* RES_BUBBLE_BLINK    = "res/bubble_blink.gif";
const char* RES_CLOSE_EYES      = "res/close_eyes.gif";
const char* RES_COMPRESSOR      = "res/compressor.gif";
const char* RES_HURRY           = "res/hurry.gif";
const char* RES_LIFE            = "res/life.gif";
const char* RES_LAUNCHER        = "res/launcher.png";
const char* RES_LOSE_PANEL      = "res/lose_panel.jpg";
const char* RES_PENGUINS        = "res/penguins.jpg";
const char* RES_SPLASH          = "res/splash.jpg";
const char* RES_VOID_PANEL      = "res/void_panel.jpg";
const char* RES_WIN_PANEL       = "res/win_panel.jpg";
const char* RES_BACKGROUND      = "res/background.jpg";
const char* RES_BUBBLE_FONT     = "res/bubble_font.gif";
//const char* RES_BUBBLE_FONT     = "res/font-hi.png";
const char* RES_APP_FROZEN_BUBBLE   = "res/app_frozen_bubble.png";
const char* RES_COMPRESSOR_BODY     = "res/compressor_body.png";

const char RES_CHARACTERS[] = {
	'!', '"', '#', '$', '%', '&', '\'', '(', ')', '*',
	'+', ',', '-', '.', '/', '0', '1', '2', '3', '4',
	'5', '6', '7', '8', '9', ':', ';', '<', '=', '>',
	'?', '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
	's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '|', '{',
	'}', '[', ']', ' ', '\\', ' ', ' ', 0};

const int RES_POSITION[]  = {
	0, 9, 16, 31, 39, 54, 69, 73, 80, 88, 96, 116, 121, 131,
	137, 154, 165, 175, 187, 198, 210, 223, 234, 246, 259,
	271, 276, 282, 293, 313, 324, 336, 351, 360, 370, 381,
	390, 402, 411, 421, 435, 446, 459, 472, 483, 495, 508,
	517, 527, 538, 552, 565, 578, 589, 602, 616, 631, 645,
	663, 684, 700, 716, 732, 748, 764, 780, 796, 812 };

const int FONT_HEIGHT = 22;

const char* SAMPLE_NAMES[] = {
    "res/newroot_solo.ogg",
    "res/lose.ogg",
    "res/noh.ogg",
    "res/rebound.ogg",
    "res/stick.ogg",
    "res/destroy_group.ogg",
    "res/applause.ogg",
    "res/hurry.ogg",
    "res/launch.ogg",
   0
};

const char* STREAM_NAMES[] = {
	"res/introzik.ogg",
	"res/frozen-mainzik-1p.ogg",
   0
};

#define RELEASE_RES(x) if(x){delete x;x = NULL;}

#define LOAD_BITMAPN_RES(x,y,z,w,h,r,c)  	\
	if((x = y::LoadFrom(z,w,h,r,c)) == NULL)\
	{\
		return false;\
	}\


#define LOAD_RES(x,y,z)  	\
	if((x = y::LoadFrom(z)) == NULL)\
	{\
		return false;\
	}\

#define LOAD_FONT_RES(x,y,z,u,v)  	\
	if((x = BitmapFont::LoadFrom(y,z,u,v)) == NULL)\
	{\
		return false;\
	}\


void FBResource_::Destroy(void)
{
	Stream::Inst().Destroy();

	RELEASE_RES( pBubble)
	RELEASE_RES( pBubbleColorBlind)
	RELEASE_RES( pBubbleFrozen)
	RELEASE_RES( pFixed)
	RELEASE_RES( pBackGround)
	RELEASE_RES( pAppFrozenBubble)
	RELEASE_RES( pBubbleBlink)

	RELEASE_RES( pFont)
	RELEASE_RES( pCloseEyes)
	RELEASE_RES( pCompressor)
	RELEASE_RES( pCompressorBody)
	RELEASE_RES( pHurry)
	RELEASE_RES( pLauncher)
	RELEASE_RES( pLife)
	RELEASE_RES( pLosePanel)
	RELEASE_RES( pPenguins)
	RELEASE_RES( pSplash)
	RELEASE_RES( pVoidPanel)
	RELEASE_RES( pWinPanel)

	RELEASE_RES(Levels)
}

bool FBResource_::Load(void)
{
	LOAD_RES( pBubble,BitmapArray,RES_BUBBLE)
	LOAD_RES( pBubbleColorBlind,BitmapArray,RES_BUBBLE2)
	LOAD_RES( pBubbleFrozen,BitmapArray,RES_FROZEN)
	LOAD_RES( pFixed,BitmapArray,RES_FIXED)

	LOAD_FONT_RES(pFont,RES_BUBBLE_FONT,RES_POSITION,RES_CHARACTERS, FONT_HEIGHT)

	LOAD_RES( pBackGround,Bitmap,RES_BACKGROUND)
	LOAD_RES( pAppFrozenBubble,Bitmap,RES_APP_FROZEN_BUBBLE)
	LOAD_RES( pBubbleBlink,Bitmap,RES_BUBBLE_BLINK)
	LOAD_RES( pCloseEyes,Bitmap,RES_CLOSE_EYES)
	LOAD_RES( pCompressor,Bitmap,RES_COMPRESSOR)
	LOAD_RES( pCompressorBody,Bitmap,RES_COMPRESSOR_BODY)
	LOAD_RES( pHurry,Bitmap,RES_HURRY)
	LOAD_RES( pLauncher,Bitmap,RES_LAUNCHER)
	LOAD_RES( pLife,Bitmap,RES_LIFE)
	LOAD_RES( pLosePanel,Bitmap,RES_LOSE_PANEL)
	LOAD_BITMAPN_RES( pPenguins,BitmapN,RES_PENGUINS,228,225,5,4)
	LOAD_RES( pSplash,Bitmap,RES_SPLASH)
	LOAD_RES( pVoidPanel,Bitmap,RES_VOID_PANEL)
	LOAD_RES( pWinPanel,Bitmap,RES_WIN_PANEL)

	//Bitmaps.Add(pBackGround);
	//Bitmaps.Add(pAppFrozenBubble);
	//Bitmaps.Add(pBubbleBlink);
	//Bitmaps.Add(pCloseEyes);
	//Bitmaps.Add(pCompressor);
	//Bitmaps.Add(pCompressorBody);
	//Bitmaps.Add(pHurry);
	//Bitmaps.Add(pLauncher);
	//Bitmaps.Add(pLife);
	//Bitmaps.Add(pLosePanel);
	//Bitmaps.Add(pPenguins);
	//Bitmaps.Add(pSplash);
	//Bitmaps.Add(pVoidPanel);
	//Bitmaps.Add(pWinPanel);

	//Bitmaps.Add(pFont->GetBitmap());

	//Bitmaps.Add(pBubble->begin(),pBubble->end());
	//Bitmaps.Add(pBubbleColorBlind->begin(),pBubbleColorBlind->end());
	//Bitmaps.Add(pBubbleFrozen->begin(),pBubbleFrozen->end());
	//Bitmaps.Add(pFixed->begin(),pFixed->end());

	Sample::Inst().LoadFrom(SAMPLE_NAMES);
	Stream::Inst().LoadFrom(STREAM_NAMES);

	srand ( time(NULL) );

	Levels = FBLevels::LoadFrom(RES_LEVELS);

	DebugMessage = new PropFont(pFont,10,50,10,10);	//here has bug ?????, where to release memory, in FBStage ? or here ?
	return true;
}

Prop* FBResource_::CreateHurryPanel()
{
	return new Prop(pHurry,203,265,90,240,false);
}

Prop* FBResource_::CreateWinPanel()
{
	return new Prop(pWinPanel,152,190,116,337,false);
}

Prop* FBResource_::CreateLosePanel()
{
	return new Prop(pLosePanel,152,190,116,337,false);
}

Prop* FBResource_::CreateBackGround()
{
	return new Prop(pBackGround,0,0,480,640);
}

Compressor* FBResource_::CreateCompressor()
{
	return new Compressor(pCompressor,pCompressorBody,0,0,480,640);
}

FBBubble* FBResource_::CreateFBBubble()
{
	return new FBBubble(pBubble,pFixed,pBubbleFrozen,pBubbleBlink);
}

PropRotation* FBResource_::CreateLauncher()
{
	return new PropRotation(pLauncher,50,50,318,406,100,100);
}

Prop* FBResource_::CreateSplash()
{
	return new Prop(pSplash,118,0,480,404);
}

int FBResource_::NextBubble()
{
	return rand() % 8 + 1;
}

int FBResource_::JumpingX()
{
	return rand() % 12 - 6;
}
int FBResource_::JumpingY()
{
	return rand() % 10 - 5;
}
int FBResource_::FallingY()
{
	return rand() % 5 + 1;
}

int STATE_VOID[]		= {0};
int STATE_TURN_LEFT[]	= {3,4,5,6};
int STATE_TURN_RIGHT[]	= {2,4,5,6};
int STATE_FIRE[]		= {1,4,5,6};
int LOST_SEQUENCE[]		= {0, 8, 9, 10, 11, 12, 13, 14};
int WON_SEQUENCE[]		= {0, 7, 6, 15, 16, 17, 18, 19};
int FULL_SEQUENCE[]		= {0, 1, 2, 3,  4,  5,  6,  7, 
						   8, 9, 10,11, 12, 13, 14, 15, 
						   16, 17, 18, 19};

PropN* FBResource_::CreatePenguins()
{
	PropN* ptr = new PropN(pPenguins,276 + 86,362 + 76,86,76);
	
	ptr->AddMode(STATE_VOID,1);
	ptr->AddMode(STATE_TURN_LEFT,4);
	ptr->AddMode(STATE_TURN_RIGHT,4);
	ptr->AddMode(STATE_FIRE,4);
	ptr->AddMode(LOST_SEQUENCE,8);
	ptr->AddMode(WON_SEQUENCE,8);
	ptr->AddMode(FULL_SEQUENCE,20);

	ptr->SetMode(SEQUENCE_VOID);
	return ptr;
	//super(new Rect(276, 362, 276 + 86, 362 + 76));	
}

PropFont* FBResource_::CreateFont()
{
	return new PropFont(pFont,185,433,10,10);
	//int y = 433;
	//int x;
	//int level = mLevelManager.getLevelIndex() + 1;
	//if (level < 10) {
	//	x = 185;
	//	mFont.paintChar(Character.forDigit(level, 10), x, y, canvas,
	//		mDisplayScale, mDisplayDX, mDisplayDY);
	//} else if (level < 100) {
	//	x = 178;
	//	x += mFont.paintChar(Character.forDigit(level / 10, 10), x, y, canvas,
	//		mDisplayScale, mDisplayDX, mDisplayDY);
	//	mFont.paintChar(Character.forDigit(level % 10, 10), x, y, canvas,
	//		mDisplayScale, mDisplayDX, mDisplayDY);
	//} else {
	//	x = 173;
	//	x += mFont.paintChar(Character.forDigit(level / 100, 10), x, y, canvas,
	//		mDisplayScale, mDisplayDX, mDisplayDY);
	//	level -= 100 * (level / 100);
	//	x += mFont.paintChar(Character.forDigit(level / 10, 10), x, y, canvas,
	//		mDisplayScale, mDisplayDX, mDisplayDY);
	//	mFont.paintChar(Character.forDigit(level % 10, 10), x, y, canvas,
	//		mDisplayScale, mDisplayDX, mDisplayDY);
	//}
}

struct F
{
    void operator()(int x, int y, int h, int w)
    {
        int op,src,dst;
        
        al_get_blender(&op,&src,&dst);
        
        al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_ALPHA);
        al_draw_filled_rectangle(x,y,x+w,y+h,al_map_rgba(137, 79, 33,100));
        
        //al_draw_filled_rectangle(this->x1, this->y1,
        //	this->x2, this->y2, bg);
        //al_draw_rectangle(this->x1 + 0.5, this->y1 + 0.5,
        //	this->x2 - 0.5, this->y2 - 0.5, fg, 0);
        
        al_set_blender(op,src,dst);
    };
    
};

PropBase* FBResource_::CreateTransparentBackground()
{
//	struct F
//	{
//		void operator()(int x, int y, int h, int w)
//		{
//			int op,src,dst;
//
//			al_get_blender(&op,&src,&dst);
//
//			al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_ALPHA);
//			al_draw_filled_rectangle(x,y,x+w,y+h,al_map_rgba(137, 79, 33,100));
//
//			//al_draw_filled_rectangle(this->x1, this->y1,
//			//	this->x2, this->y2, bg);
//			//al_draw_rectangle(this->x1 + 0.5, this->y1 + 0.5,
//			//	this->x2 - 0.5, this->y2 - 0.5, fg, 0);
//
//			al_set_blender(op,src,dst);
//		};
//
//	};

	//PropBase<F>* prop = new PropBase<F>(118,0,480,404);//352-42,437-61
	PropDrawing<F>* prop = new PropDrawing<F>(118+42,0+61,437-61,362-42);//362-42,437-61
	return prop;
}

PropWidget* FBResource_::CreateEntryMenu(int levelnum, bool music_on, bool sound_on)
{
	al_set_new_bitmap_flags(ALLEGRO_NO_PREMULTIPLIED_ALPHA);

//	ALLEGRO_FONT *font = al_load_font(util_get_path("res/fixed_font.tga"), 0, 0);
	ALLEGRO_FONT *font = NULL;
	//if(!font)
	//{
	//	return NULL;
	//}

	Theme* theme = new Theme(font);
	PropWidget* prop = new PropWidget(theme,0,0,300,300);

	Button* start = new Button("START GAME");
	start->id = MENU_ID_START;
	UpDownControl* level  = new UpDownControl("LEVEL", levelnum, 1, Levels->GetMax());
	level->id = MENU_ID_LEVEL;
	ToggleButton* music  = new ToggleButton("MUSIC",music_on);
	music->id = MENU_ID_MUSIC;
	ToggleButton* sound  = new ToggleButton("SOUND",sound_on);
	sound->id = MENU_ID_SOUND;
	Button* exit  = new Button("EXIT");
	exit->id = MENU_ID_EXIT;
	int button_start_x = 190;
	int button_start_y = 100;
	int button_inc_y = 10;
	int button_w = 200;
	int button_h = 50;

	prop->add(start,button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	prop->add(level, button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	prop->add(music, button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	prop->add(sound, button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	prop->add(exit, button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	return prop;
}

PropWidget* FBResource_::CreateMainMenu(int levelnum, bool music_on, bool sound_on)
{
	al_set_new_bitmap_flags(ALLEGRO_NO_PREMULTIPLIED_ALPHA);

//	ALLEGRO_FONT *font = al_load_font(util_get_path("res/fixed_font.tga"), 0, 0);
	ALLEGRO_FONT *font = NULL;
	//if(!font)
	//{
	//	return NULL;
	//}

	Theme* theme = new Theme(font);
	PropWidget* prop = new PropWidget(theme,0,0,300,300,false);

	Button* start = new Button("RESUME GAME");
	start->id = MENU_ID_RESUME;
	ToggleButton* music  = new ToggleButton("MUSIC",music_on);
	music->id = MENU_ID_MUSIC;
	ToggleButton* sound  = new ToggleButton("SOUND",sound_on);
	sound->id = MENU_ID_SOUND;
	Button* exit  = new Button("EXIT");
	exit->id = MENU_ID_EXIT;
	int button_start_x = 190;
	int button_start_y = 100;
	int button_inc_y = 10;
	int button_w = 200;
	int button_h = 50;

	prop->add(start,button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	prop->add(music, button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	prop->add(sound, button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	prop->add(exit, button_start_x,button_start_y,button_w,button_h);
	button_start_y+=button_h;
	button_start_y+=button_inc_y;

	return prop;
}


Prop* SplashResource_::CreateAppFrozenBubble()
{
	return new Prop(pAppFrozenBubble,(640-280)/2+(280-128)/2,190+(156-128)/2,128,128);
}

Prop* SplashResource_::CreateVoidPanel()
{
	return new Prop(pVoidPanel,(640-280)/2,190,156,280);
}

PropFont* SplashResource_::CreateFont()
{
	return new PropFont(pFont,185,433,10,10);
}

void SplashResource_::Destroy(void)
{
	RELEASE_RES( pFont)
	RELEASE_RES( pVoidPanel)
	RELEASE_RES( pAppFrozenBubble)
}

bool SplashResource_::Load(void)
{
	LOAD_FONT_RES(pFont,RES_BUBBLE_FONT,RES_POSITION,RES_CHARACTERS, FONT_HEIGHT)
	LOAD_RES( pVoidPanel,Bitmap,RES_VOID_PANEL)
	LOAD_RES( pAppFrozenBubble,Bitmap,RES_APP_FROZEN_BUBBLE)

	return true;
}

