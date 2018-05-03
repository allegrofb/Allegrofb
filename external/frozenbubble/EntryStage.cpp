#include "EntryStage.hpp"
#include "FBResource.hpp"
#include "GlobalSetting.hpp"


bool EntryStage::OnInit()
{
	SetCenterOffset(GlobalSetting::Inst().StageOffsetX,GlobalSetting::Inst().StageOffsetY);
	Add(FBResource::Inst().CreateBackGround());
	Add(FBResource::Inst().CreateSplash());
	Add(FBResource::Inst().CreateTransparentBackground());

	level = GlobalSetting::Inst().LevelNum;
	music_on = GlobalSetting::Inst().MusicOn;
	sound_on = GlobalSetting::Inst().SoundOn;
	myMenu = FBResource::Inst().CreateEntryMenu(level,music_on,sound_on);Add(myMenu);

    myBubbleLevel = new PropBitmap(0,0,320,640,false);  //background resolution: 640x480
                                                        //display: 320x480
                                                        //bitmap:  640x320
    Add(myBubbleLevel);
    
	if(music_on)
		Stream::Inst().Play(INTROZIK,ALLEGRO_PLAYMODE_LOOP);
    
	myState = STATE_NORMAL;
	myTransition.Init(GlobalSetting::Inst().DisplayWidth,GlobalSetting::Inst().DisplayHeight,
		Tube3DTransition::TO_LEFT_FRONT,GlobalSetting::Inst().FrameRate,NULL);
	return true;
}

void EntryStage::DisplayLevelOutline(int level)
{        
    FBBubble* bubble = FBResource::Inst().CreateFBBubble();
    bubble->InitLevel(level);
    myBubbleLevel->Update(bubble,0,0,1);
    delete bubble;
    
    myBubbleLevel->SetVisible(true,GlobalSetting::Inst().GetFrame(1.0));
}

void  EntryStage::OnRelease()
{
	Stream::Inst().Stop(INTROZIK);
}

int EntryStage::OnUpdate()
{
	if(this->myState != STATE_NORMAL)
		return 0;

	for(std::list<Widget*>::const_iterator i = myMenu->begin(),
		m = myMenu->end();i != m;i++)
	{
		if((*i)->id == MENU_ID_START)
		{
			if(((Button*)(*i))->is_clicked())
				Close(2);
		}
		else if((*i)->id == MENU_ID_LEVEL)
		{
			if(((UpDownControl*)(*i))->is_changed())
            {
				int level = GlobalSetting::Inst().LevelNum;
				this->myTransition.UpdateFirst(this,-160,0,1);
				GlobalSetting::Inst().LevelNum = ((UpDownControl*)(*i))->get_value();
				FBBubble* bubble = FBResource::Inst().CreateFBBubble();
				bubble->InitLevel(GlobalSetting::Inst().LevelNum);
				this->myTransition.UpdateSecond(bubble,-160,0,1);
				delete bubble;

				if(level > GlobalSetting::Inst().LevelNum || GlobalSetting::Inst().LevelNum == 1)
				{
					this->myTransition.CalculateCamera(Tube3DTransition::TO_RIGHT_FRONT);
				}
				else
				{
					this->myTransition.CalculateCamera(Tube3DTransition::TO_LEFT_FRONT);
				}

				this->stateTransition = STATE_NORMAL;
				myState = STATE_TRANSITION;
				this->myTransition.Prepare();
            }
		}
		else if((*i)->id == MENU_ID_MUSIC)
		{
			if(music_on != ((ToggleButton*)(*i))->get_toggle())
			{
				music_on = !music_on;
				if(music_on)
					Stream::Inst().Play(INTROZIK,ALLEGRO_PLAYMODE_LOOP);
				else 
					Stream::Inst().Stop(INTROZIK);
				GlobalSetting::Inst().MusicOn = music_on;
			}
		}
		else if((*i)->id == MENU_ID_SOUND)
		{
			if(sound_on != ((ToggleButton*)(*i))->get_toggle())
			{
				sound_on = !sound_on;
				if(sound_on)
					Sample::Inst().Play(LAUNCH);
				GlobalSetting::Inst().SoundOn = sound_on;
			}
		}
		else if((*i)->id == MENU_ID_EXIT)
		{
			if(((Button*)(*i))->is_clicked())
			Close(-1);
		}
	}
	return 0;
}


void EntryStage::Render()
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

bool EntryStage::OnMouseDown(int x, int y)
{
    myMenu->OnMouseDown(x,y);
	return false;
}

bool EntryStage::OnMouseMoving(int x, int y)
{
	myMenu->OnMouseMoving(x,y);
	return false;
}

bool EntryStage::OnMouseUp(int x, int y)
{
	myMenu->OnMouseUp(x,y);
	return false;
}


