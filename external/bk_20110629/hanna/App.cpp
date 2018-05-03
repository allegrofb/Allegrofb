#include "App.hpp"

int App::Run()
{

	if(!Init())
	{
		debug_message("Error Init\n");
		return 1;
	}
    
    if(!OnInit())
	{
		debug_message("Error OnInit\n");
		return 1;
	}

	//volatile int inx = 0;

	//for(;;)
	//{
	//	inx = stageVector[inx]->Run(myDisplay);

	//	if(inx < 0 || inx > 1)
	//	{
	//		break;
	//	}
	//}

	std::vector<Stage*>::const_iterator i = stageVector.begin();
	if(i != stageVector.end())
	{
		Stage* ptr = *i;
		while(1)
		{
			int r = ptr->Run(myDisplay);
			if(r >= (int)stageVector.size() || r < 0)
				break;
			ptr = stageVector[r];
		}
	}

	OnRelease();
	Release();

	al_destroy_display(myDisplay);

	return 0;
}

void App::Add(Stage* p)
{
	stageVector.push_back(p);	
}

bool App::Init()
{
    if (!al_init()) 
    {
        debug_message("Error in initialization.\n");
        return false;
    }
    
	al_init_primitives_addon();
	al_install_mouse();
    al_install_keyboard();
	al_init_font_addon();
	al_init_image_addon();
	al_init_acodec_addon();

	if (!al_install_audio()) 
	{
		debug_message("Could not init sound!\n");
		return false;
	}

	//myDisplay = al_create_display(640, 480);
	myDisplay = al_create_display(320, 480);
    //in the windows, this is real resolution	
    //in the iphone, it will adjust width and height according to orientation
    
    
	//myDisplay = al_create_display(320, 480);	
	//hyjiang, we can set different resolution with game, then drag and zoom in/out screen
    
	if (!myDisplay) 
	{
		debug_message("Error creating display\n");
		return false;
	}
    
	return true;
}

void App::Release()
{
	for(std::vector<Stage*>::iterator i = stageVector.begin();
		i != stageVector.end();i++)
	{
		delete *i;
	}
	stageVector.clear();

   /* Sample data and other objects will be automatically freed. */
   al_uninstall_audio();
}

