#include "EntryStage.hpp"
#include "FBResource.hpp"

bool EntryStage::OnInit()
{
	SetCenterOffset(-160,0);
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

	return true;
}

void EntryStage::DisplayLevelOutline(int level)
{        
    FBBubble* bubble = FBResource::Inst().CreateFBBubble();
    bubble->InitLevel(level);
    myBubbleLevel->Update(bubble,0,0,1);
    delete bubble;
    
    myBubbleLevel->SetVisible(true,30);
}

void EntryStage::OnBackGround()
{
    
}

void EntryStage::OnForeGround()
{

}

void  EntryStage::OnRelease()
{
	Stream::Inst().Stop(INTROZIK);
}

void EntryStage::OnOrientation(int orientation)
{
    ALLEGRO_TRANSFORM t;
    float x = 0;
    float y = 0;
    float sx = 1;
    float sy = 1;
    float theta = 0;    
        
    return;
    
    #ifdef WIN32
    return;
    #endif
    
    if(orientation == ALLEGRO_DISPLAY_ORIENTATION_0_DEGREES)
    {
        // upright and the home button at the bottom
        x = -480;
        y = 160;
        sx = 1;
        sy = 1;
        theta = -3.14159165/2;
        //theta = 0;
        al_identity_transform(&t);
        al_translate_transform(&t,x,y);
        al_scale_transform(&t, sx, sy);
        al_rotate_transform(&t, theta);
        AssignTrans(t);
                
    }
    else if(orientation == ALLEGRO_DISPLAY_ORIENTATION_180_DEGREES)
    {
        //upright and the home button at the top
        
    }
    else if(orientation == ALLEGRO_DISPLAY_ORIENTATION_90_DEGREES)
    {
        //upright and the home button on the left side
        
    }
    else if(orientation == ALLEGRO_DISPLAY_ORIENTATION_270_DEGREES)
    {
        //upright and the home button on the right side
        x = 0;
        y = 0;
        sx = 1;
        sy = 1;
        theta = 0;    
        
        al_identity_transform(&t);
        al_translate_transform(&t,x,y);
        al_scale_transform(&t, sx, sy);
        al_rotate_transform(&t, theta);
        AssignTrans(t);
    }
}

int EntryStage::OnUpdate()
{
	for(std::list<Widget*>::const_iterator i = myMenu->all_widgets.begin(),
		m = myMenu->all_widgets.end();i != m;i++)
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
				GlobalSetting::Inst().LevelNum = ((UpDownControl*)(*i))->get_value();
                DisplayLevelOutline(GlobalSetting::Inst().LevelNum);
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

void EntryStage::OnChar(int keycode, int unichar)
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


bool EntryStage::OnMouseDown(int x, int y)
{
    //if(x < 100 && y < 100)
    //{
    //    OnChar(0, 'a');
    //}
    //else if(x < 400 && y < 100)
    //{
    //    OnChar(0, 's');        
    //}
    //else if(x < 640 && y < 100)
    //{
    //    OnChar(0, 'd');        
    //}
    //else if(x < 100 && y < 200)
    //{
    //    OnChar(0, 'f');        
    //}
    //else if(x < 400 && y < 200)
    //{
    //    OnChar(0, 'g');        
    //}
    //else if(x < 640 && y < 200)
    //{
    //    OnChar(0, 'h');        
    //}
    //else if(x < 100 && y < 480)
    //{
    //    OnChar(0, 'j');        
    //}
    
    
    
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

int SplashStage::OnUpdate()
{
	myLoader->Start();

	return 1;
}

bool SplashStage::OnInit()
{
	if(!SplashResource::Inst().Load())
		return false;

	SetCenterOffset(-160,0);

	Add(SplashResource::Inst().CreateVoidPanel());
	Add(SplashResource::Inst().CreateAppFrozenBubble());
	myLevelStr = SplashResource::Inst().CreateFont();Add(myLevelStr);
	myLevelStr->Assign("LOADING...");

	return true;
}

void SplashStage::OnRelease()
{
	SplashResource::Inst().Destroy();
	delete myLoader;
}




