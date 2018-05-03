#include "FBStage.hpp"
#include "FBResource.hpp"

bool FBStage::OnInit()
{
	SetCenterOffset(-160,0);
	Add(FBResource::Inst().CreateBackGround());
	myLauncher = FBResource::Inst().CreateLauncher();Add(myLauncher);
	myBubble = FBResource::Inst().CreateFBBubble();Add(myBubble);
	myPenguins = FBResource::Inst().CreatePenguins();Add(myPenguins);
	myPenguins->SetMode(SEQUENCE_VOID);
	myLevelStr = FBResource::Inst().CreateFont();Add(myLevelStr);
	myCompressor = FBResource::Inst().CreateCompressor();Add(myCompressor);	
	myHurryPanel = FBResource::Inst().CreateHurryPanel();Add(myHurryPanel);	
	myWinPanel = FBResource::Inst().CreateWinPanel();Add(myWinPanel);
	myLosePanel = FBResource::Inst().CreateLosePanel();Add(myLosePanel);
	myTransparentBackground = FBResource::Inst().CreateTransparentBackground();Add(myTransparentBackground);
	myTransparentBackground->SetVisible(false);

	currentLevel = GlobalSetting::Inst().LevelNum;
	musicOn = GlobalSetting::Inst().MusicOn;
	soundOn = GlobalSetting::Inst().SoundOn;
	myMenu = FBResource::Inst().CreateMainMenu(currentLevel,musicOn,soundOn);Add(myMenu);

	Init(currentLevel);

	myMenuFlag = false;

	if(musicOn)
	Stream::Inst().Play(MAINZIK);

	//remove it, hyjiang
	//Add(FBResource::Inst().DebugMessage);
	//FBResource::Inst().DebugMessage->Assign(10,11);
	return true;
}

void  FBStage::UpdateMenu()
{
	for(std::list<Widget*>::const_iterator i = myMenu->all_widgets.begin(),
		m = myMenu->all_widgets.end();i != m;i++)
	{
		if((*i)->id == MENU_ID_LEVEL)
		{

		}
	}
}

void  FBStage::OnRelease()
{
	Stream::Inst().Stop(MAINZIK);
}

void FBStage::Init(int level)
{
	hurryTime = 0;
	angle = 0;
	fixedBubbles = 0;
	endOfGame = 0;
	blinkDelay = 0;
	myState = STATE_NORMAL;

	myLevelStr->Assign(level);
	myPenguins->SetMode(SEQUENCE_VOID);
	myBubble->InitLevel(level);
	myCompressor->Reset();
	myHurryPanel->SetVisible(false);
	myWinPanel->SetVisible(false);
	myLosePanel->SetVisible(false);
}


void FBStage::StateNormal()
{
	int result = myBubble->Update();

	if(result == 1)
	{
		fixedBubbles++;
		if (fixedBubbles == 8) 
		{
			fixedBubbles = 0;
			myBubble->MoveDown();
			myCompressor->MoveDown();
            if(GlobalSetting::Inst().SoundOn)
                Sample::Inst().Play(NEWROOT_SOLO);
		}

		if(myBubble->CheckFail())
		{
			//fail
			myLosePanel->SetVisible(true, 30);
            if(GlobalSetting::Inst().SoundOn)
                Sample::Inst().Play(LOSE);
			myPenguins->SetMode(SEQUENCE_LOST);
			endOfGame = 1;
			myBubble->SetFail();
			myState = STATE_LOSE;
		}

	}
	else if(result == 2)
	{
		if(myBubble->CheckSuccess())
		{
			//success
			myWinPanel->SetVisible(true,30);
            if(GlobalSetting::Inst().SoundOn)
                Sample::Inst().Play(APPLAUSE);
			myPenguins->SetMode(SEQUENCE_WON);
			endOfGame = 1;
			myState = STATE_WIN;
			myBubble->SetMovingOnesInvalid();
		}
	}
	else if(result == 3)
	{
		//fail
		myLosePanel->SetVisible(true, 30);
        if(GlobalSetting::Inst().SoundOn)
            Sample::Inst().Play(LOSE);
		myPenguins->SetMode(SEQUENCE_LOST);
		endOfGame = 1;
		myState = STATE_LOSE;
	}

	if(!myBubble->MovingBubbleExist() && !endOfGame)
	{
		hurryTime++;
		if (hurryTime>=240) 
		{
			if (hurryTime % 40 == 10) 
			{
				myHurryPanel->SetVisible(true);
                if(GlobalSetting::Inst().SoundOn)
                    Sample::Inst().Play(HURRY);
			} 
			else if (hurryTime % 40 == 35) 
			{
				myHurryPanel->SetVisible(false);
			}
		}

		if(hurryTime > 480)	//
		{
			myBubble->Fire(angle);
			hurryTime = 0;
		}
	}

	if(!endOfGame)
	{
		if (fixedBubbles == 6) 
		{
			if (blinkDelay < 15) 
			{
				myBubble->SetBlink(blinkDelay);
			}

			blinkDelay++;
			if (blinkDelay >= 40) 
			{
				blinkDelay = 0;
			}
		} 
		else if (fixedBubbles == 7) 
		{
			if (blinkDelay < 15) 
			{
				myBubble->SetBlink(blinkDelay);
			}

			blinkDelay++;
			if (blinkDelay >= 25) 
			{
				blinkDelay = 0;
			}
		}
	}
}

void FBStage::StateWin()
{
    myBubble->Update();
}

void FBStage::StateLose()
{
	
}

void FBStage::StateMenu()
{
	for(std::list<Widget*>::const_iterator i = myMenu->all_widgets.begin(),
		m = myMenu->all_widgets.end();i != m;i++)
	{
		if((*i)->id == MENU_ID_RESUME)
		{
			if(((Button*)(*i))->is_clicked())
			{
				myState = myMenuStateBk;
				myMenu->SetVisible(false);
				myTransparentBackground->SetVisible(false);
			}
		}
		else if((*i)->id == MENU_ID_MUSIC)
		{
			if(musicOn != ((ToggleButton*)(*i))->get_toggle())
			{
				musicOn = !musicOn;
				if(musicOn)
					Stream::Inst().Play(MAINZIK);
				else 
					Stream::Inst().Stop(MAINZIK);

				GlobalSetting::Inst().MusicOn = musicOn;
			}
		}
		else if((*i)->id == MENU_ID_SOUND)
		{
			if(soundOn != ((ToggleButton*)(*i))->get_toggle())
			{
				soundOn = !soundOn;
				if(soundOn)
					Sample::Inst().Play(LAUNCH);
				GlobalSetting::Inst().SoundOn = soundOn;       
			}
		}
		else if((*i)->id == MENU_ID_EXIT)
		{
			if(((Button*)(*i))->is_clicked())
			Close();
		}
	}
}

void FBStage::OnAction()
{

}


int FBStage::OnUpdate()
{
	if(myMenuFlag)
	{
		myMenuFlag = false;
		myTransparentBackground->SetVisible(true);
		myMenu->SetVisible(true);
		myMenuStateBk = myState;
		myState = STATE_MENU;
		UpdateMenu();
	}

	if(myState == STATE_NORMAL)	//0 - normal, 1 - win, 2 - lose
	{
		StateNormal();
	}
	else if(myState == STATE_WIN)
	{
		StateWin();
	}
	else if(myState == STATE_LOSE)
	{
		StateLose();
	}
	else if(myState == STATE_MENU)
	{
		StateMenu();
	}
	return 0;
}

void FBStage::OnChar(int keycode, int unichar)
{
	static int mode = 0;
	static ALLEGRO_TRANSFORM t;
	static float x = 0;
	static float y = 0;
	static float sx = 1;
	static float sy = 1;
	static float theta = 0;

	if (keycode == ALLEGRO_KEY_ESCAPE)
	{
		//Close();
		//switch to menu
	}
	else if (unichar == 't')
	{
		OnShakeGesture();
	}
	else if (unichar == '1')
	{
		angle += ANGLE(5);
		myPenguins->RunOneTime(SEQUENCE_TURN_LEFT);
		myLauncher->SetRotation(angle);
	}
	else if (unichar == '2')
	{
		angle -= ANGLE(5);
		myPenguins->RunOneTime(SEQUENCE_TURN_RIGHT);
		myLauncher->SetRotation(angle);
	}
	else if (unichar == '3')
	{
		myPenguins->SetMode(mode);
		mode++;
		if(mode > 7)
			mode = 0;
	}
	else if (unichar == '0')
	{
		myPenguins->RunOneTime(SEQUENCE_FIRE);
		myBubble->Fire(angle);
		hurryTime = 0;
		myHurryPanel->SetVisible(false);
	}
	else if (unichar == 'a')
	{
		x = 0;
		y = 0;
		sx = 1;
		sy = 1;
		theta = 0;
	}
	else if (unichar == 's')
	{
		x += 10;
	}
	else if (unichar == 'd')
	{
		y += 10;
	}
	else if (unichar == 'f')
	{
		sx += 0.1;
	}
	else if (unichar == 'g')
	{
		sy += 0.1;
	}
	else if (unichar == 'h')
	{
		theta += 0.1;
	}
	else if (unichar == 'j')
	{
		x = -480;
		theta = -3.14159165/2;
	}
	else if (unichar == 'k')
	{
		sx = 2;
		x  = -160;		//153-485
	}
	
	al_identity_transform(&t);
	al_translate_transform(&t,x,y);
	al_scale_transform(&t, sx, sy);
	//theta, clock roation according to 0,0
	//translate and scale according to rotation
	al_rotate_transform(&t, theta);
	AssignTrans(t);

	return;
}

bool FBStage::OnMouseDown(int x, int y)
{
	if(myState == STATE_MENU)
	{
		myMenu->OnMouseDown(x,y);
	}
	else
	{

		UpdateCenterOffset(x,y);

#ifndef WIN32
		if(STATE_NORMAL == this->myState)
		{
			float tmp = 0;
			myBubble->CalculateAngle(x,y,tmp);
			myLauncher->SetRotation(tmp);
			if(tmp > angle)
				myPenguins->RunOneTime(SEQUENCE_TURN_LEFT);
			else
				myPenguins->RunOneTime(SEQUENCE_TURN_RIGHT);

			angle = tmp;
			return false;
		}    
#endif

	}
	return false;
}

bool FBStage::OnMouseMoving(int x, int y)
{
	if(myState == STATE_MENU)
	{
		myMenu->OnMouseMoving(x,y);
	}
	else
	{
		UpdateCenterOffset(x,y);

#ifndef WIN32
		if(STATE_NORMAL == this->myState)
		{
			float tmp = 0;
			myBubble->CalculateAngle(x,y,tmp);
			myLauncher->SetRotation(tmp);
			if(tmp > angle)
				myPenguins->RunOneTime(SEQUENCE_TURN_LEFT);
			else
				myPenguins->RunOneTime(SEQUENCE_TURN_RIGHT);

			angle = tmp;
			return false;
		}    
#endif
	}
	return false;
}

bool FBStage::OnMouseUp(int x, int y)
{
	if(myState == STATE_MENU)
	{
		myMenu->OnMouseUp(x,y);
	}
	else
	{
		UpdateCenterOffset(x,y);
		if(STATE_NORMAL == this->myState)
		{
			myPenguins->RunOneTime(SEQUENCE_FIRE);
			myBubble->Fire(x,y,angle);
			myLauncher->SetRotation(angle);
			hurryTime = 0;
			myHurryPanel->SetVisible(false);
			return false;
		}
		else if(STATE_WIN == this->myState)
		{
			++currentLevel;
			if(currentLevel > FBResource::Inst().Levels->GetMax())
				currentLevel = 1;
			Init(currentLevel);
			GlobalSetting::Inst().LevelNum = currentLevel;
		}
		else if(STATE_LOSE == this->myState)
		{
			Init(currentLevel);
			GlobalSetting::Inst().LevelNum = currentLevel;
		}
	}
	return false;
}

void FBStage::OnShakeGesture()
{
	if(myState != STATE_MENU)
	{
		myMenuFlag = true;
	}
}