#include "Stage.hpp"
#include "Alo.hpp"

Stage::Stage()
	: myCenterOffsetX(0),myCenterOffsetY(0),myZoom(1)
{

}

void Stage::Add(Drawable* p)
{
	drawableList.push_back(p);
}

void Stage::Release()
{
	for(std::vector<Drawable*>::const_iterator i = drawableList.begin();
		i != drawableList.end();i++)
	{
		delete *i;
	}

	drawableList.clear();
}

bool Stage::Init()
{
	//al_identity_transform(&myTransform);
    
	myMouseButtonDown = false;
	myExitCode = 0;
	return true;
}

void Stage::GetCenterOffset(int& x, int& y)
{
	x = myCenterOffsetX;
	y = myCenterOffsetY;
}

void Stage::SetCenterOffset(int x, int y)	//update stage center in the display
{
	myCenterOffsetX = x;
	myCenterOffsetY = y;
}

void Stage::Close(int code)
{
	myExitCode = code;
}

void Stage::AssignTrans(ALLEGRO_TRANSFORM& t)
{
	myTransform = t;
}

void Stage::Render(int x, int y, double zoom)
{
	for(std::vector<Drawable*>::iterator i = drawableList.begin();
		i != drawableList.end();i++)
	{
		if(*i == NULL)
			continue;
		else
			(*i)->Render(x,y,zoom);
	}
}

void Stage::Render()
{
	Render(myCenterOffsetX,myCenterOffsetY,myZoom);
}

int Stage::Run(ALLEGRO_DISPLAY *display)
{
	bool redraw = true;
	ALLEGRO_TIMER *timer;
	volatile int result = 0;
    
    if(!(timer = al_create_timer(setting->GetTime(1))))	//frozenbubble 25Hz
	{
		debug_message("Error create timer\n");
		return -1;
	}
	
    myQueue = al_create_event_queue();
    al_register_event_source(myQueue, al_get_keyboard_event_source());
    al_register_event_source(myQueue, al_get_display_event_source(display));
    al_register_event_source(myQueue, al_get_timer_event_source(timer));
    al_register_event_source(myQueue, al_get_mouse_event_source());

	Init();

	if(!OnInit())
	{
		debug_message("Error stage OnInit\n");
		return -1;
	}

    al_start_timer(timer);
	while (result == 0) 
	{
		ALLEGRO_EVENT event;

		if(myExitCode)
		{
			result = myExitCode;
			break;
		}

		al_wait_for_event(myQueue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
		}
		else if(event.type == ALLEGRO_EVENT_DISPLAY_ORIENTATION)
		{
            OnOrientation(event.display.orientation);
            // upright and the home button at the bottom
            //ALLEGRO_DISPLAY_ORIENTATION_0_DEGREES

            //upright and the home button at the top
            //ALLEGRO_DISPLAY_ORIENTATION_180_DEGREES

            //upright and the home button on the left side
            //ALLEGRO_DISPLAY_ORIENTATION_90_DEGREES

            //upright and the home button on the right side
            //ALLEGRO_DISPLAY_ORIENTATION_270_DEGREES

            // parallel to the ground with the screen facing upwards.
            //ALLEGRO_DISPLAY_ORIENTATION_FACE_UP

            //parallel to the ground with the screen facing downwards.
            //ALLEGRO_DISPLAY_ORIENTATION_FACE_DOWN

		}
		else if(event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT)
		{
			//become to background
			#ifdef ALLEGRO_IPHONE
            this->OnBackground();
            al_stop_timer(timer);
            al_iphone_program_has_halted();
			#endif
		}
		else if(event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
		{
			//become to foreground
            #ifdef ALLEGRO_IPHONE
            this->OnForeground();
            al_start_timer(timer);
            #endif
		}
		else if (event.type == ALLEGRO_EVENT_KEY_CHAR) 
		{
			OnChar(event.keyboard.keycode,event.keyboard.unichar);
        }
		else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			myMouseButtonDown = true;
			//or touch begin
			OnMouseDown(event.mouse.x,event.mouse.y);
		}
		else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			myMouseButtonDown = false;
			//or touch end
			OnMouseUp(event.mouse.x,event.mouse.y);
		}
		else if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			if(myMouseButtonDown)
			//or touch continous
			OnMouseMoving(event.mouse.x,event.mouse.y);
		}
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            myExitCode = result = -1;
            break;
        }
		else if(event.type == ALLEGRO_EVENT_SHAKE)
		{
			OnShakeGesture();
		}

		if (redraw && al_is_event_queue_empty(myQueue)) 
		{
			redraw = false;
			al_clear_to_color(al_map_rgb_f(0, 0, 0));
			Render();
			al_flip_display();
			if((result = OnUpdate()))   //this OnUpdate function place is not good
			{
				myExitCode = result;
				break;
			}
		}
	}
	al_destroy_timer(timer);

	OnRelease();

	Release();

	al_destroy_event_queue(myQueue);

	return result;
}
