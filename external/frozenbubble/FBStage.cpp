#include "FBStage.hpp"
#include "FBResource.hpp"
#include "GlobalSetting.hpp"

bool FBStage::OnInit()
{
	SetCenterOffset(GlobalSetting::Inst().StageOffsetX,GlobalSetting::Inst().StageOffsetY);
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
	myState = STATE_NORMAL;

	if(musicOn)
		Stream::Inst().Play(MAINZIK);

	myTransition.Init(GlobalSetting::Inst().DisplayWidth,GlobalSetting::Inst().DisplayHeight,
		Tube3DTransition::TO_LEFT,GlobalSetting::Inst().FrameRate,NULL);

	return true;
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

	if(result == FBBubble::UPDATE_ONEBUBBLE_FIXED)
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
	else if(result == FBBubble::UPDATE_FALLING)
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
	else if(result == FBBubble::UPDATE_FAILED)
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
	(void)0;
}

void FBStage::StateMenu()
{
	for(std::list<Widget*>::const_iterator i = myMenu->begin(),
		m = myMenu->end();i != m;i++)
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

void FBStage::Render()
{
	if(this->myState != STATE_TRANSITION)
	{
		Stage::Render();
	}
	else
	{
		this->myTransition.Render(0,0,1);
		if(this->myTransition.IsFinished())
		{
			myTransition.Finish();
			myState = this->stateTransition;
		}
	}
}

int FBStage::OnUpdate()
{
	if(myState == STATE_NORMAL)	
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
    (void)unichar;
	if (keycode == ALLEGRO_KEY_ESCAPE)
	{
        OnShakeGesture();
	}
	return;
}

void FBStage::OnShakeGesture()
{
	if(myState != STATE_MENU)
	{
        myTransparentBackground->SetVisible(true);
        myMenu->SetVisible(true);
        myMenuStateBk = myState;
        myState = STATE_MENU;
	}
}

void FBStage::UpdateCenterOffset(int &x, int &y)
{
    int os_x,os_y;
    GetCenterOffset(os_x, os_y);
    x -= os_x;
    y -= os_y;
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

#ifdef ALLEGRO_IPHONE
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
            int os_x,os_y;
            GetCenterOffset(os_x,os_y);
			this->myTransition.UpdateFirst(this,os_x,os_y,1);
			++currentLevel;
			if(currentLevel > FBResource::Inst().Levels->GetMax())
				currentLevel = 1;
			Init(currentLevel);
			GlobalSetting::Inst().LevelNum = currentLevel;
			this->myTransition.UpdateSecond(this,os_x,os_y,1);
			this->stateTransition = STATE_NORMAL;
			myState = STATE_TRANSITION;
			this->myTransition.CalculateCamera(Tube3DTransition::TO_LEFT);
			this->myTransition.Prepare();
		}
		else if(STATE_LOSE == this->myState)
		{
            int os_x,os_y;
            GetCenterOffset(os_x,os_y);            
			this->myTransition.UpdateFirst(this,os_x,os_y,1);
			Init(currentLevel);
			GlobalSetting::Inst().LevelNum = currentLevel;
			this->myTransition.UpdateSecond(this,os_x,os_y,1);
			this->stateTransition = STATE_NORMAL;
			myState = STATE_TRANSITION;
			this->myTransition.CalculateCamera(Tube3DTransition::TO_RIGHT);
			this->myTransition.Prepare();
		}
	}
	return false;
}
