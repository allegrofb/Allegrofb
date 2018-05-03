#include "Widget.hpp"

#include <iostream>
#include <string>
#include <algorithm>

#include "FBResource.hpp"

namespace 
{
	class SaveState
	{
		ALLEGRO_STATE state;

	public:
		SaveState(int save=ALLEGRO_STATE_ALL)
		{
			al_store_state(&state, save);
		}
		~SaveState()
		{
			al_restore_state(&state);
		}
	};

	class UString
	{
		ALLEGRO_USTR_INFO info;
		ALLEGRO_USTR *ustr;

	public:
		UString(const ALLEGRO_USTR *s, int first, int end = -1)
		{
			if (end == -1)
				end = al_ustr_size(s);
			ustr = al_ref_ustr(&info, s, first, end);
		}

		// Conversion
		operator const ALLEGRO_USTR *() const
		{
			return ustr;
		}
	};

};


Widget::Widget()
    :grid_x(0),grid_y(0),grid_w(0),grid_h(0),x1(0),y1(0),x2(0),y2(0),selected(TOUCH_OFF)
{
}

void Widget::set_xy_wh(int x,int y,int w,int h)
{
    grid_x = x;
    grid_y = y;
    grid_w = w;
    grid_h = h;
}

void Widget::set_clippling_rectangle()
{
    al_set_clipping_rectangle(x1, y1, width(), height());
}

void Widget::configure(int offsetx,int offsety,double zoom)
{
    (void)zoom;
	this->x1 = this->grid_x + offsetx;
	this->y1 = this->grid_y + offsety;
	this->x2 = (this->grid_x + this->grid_w) + offsetx;
	this->y2 = (this->grid_y + this->grid_h) + offsety;
}

bool Widget::contains(int x, int y)
{
	return (x >= this->x1 && y >= this->y1 && x <= this->x2 && y <= this->y2);
}


Label::Label(std::string text, bool centred) :
text(text),
	centred(centred)
{
}

void Label::draw()
{
	SaveState state;
/*
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	if (centred) {
		al_draw_text(theme->font, theme->fg, (this->x1 + this->x2 + 1)/2,
			this->y1, ALLEGRO_ALIGN_CENTRE, this->text.c_str());
	}
	else {
		al_draw_text(theme->font, theme->fg, this->x1, this->y1, 0, this->text.c_str());
	}*/
}

void Label::set_text(std::string new_text)
{
	this->text = new_text;
}

Button::Button(std::string text)
	:text(text),flash_time(0),clicked_flag(false)
{

}

void Button::on_selected(int x, int y)
{
	(void)x;(void)y;
	if(selected == TOUCH_ON)
		flash_time = DELAY_TIME;
}

bool Button::is_clicked()
{
	bool r = clicked_flag;
	clicked_flag = false; 
	return r;
}

void Button::draw()
{
	ALLEGRO_COLOR fg;
	ALLEGRO_COLOR bg;
	SaveState state;

	if (this->flash_time) 
	{
		bg = al_map_rgba(255, 255, 255,100);
		fg = al_map_rgb(0, 0, 0);
	}
	else {
		bg = al_map_rgba(255, 255, 255,100);
		fg = al_map_rgb(0, 0, 0);
	}

	int op,src,dst;

	al_get_blender(&op,&src,&dst);

	if (this->flash_time) 
	{
		al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_ALPHA);
		//al_draw_filled_rectangle(x,y,x+w,y+h,al_map_rgba(255, 255, 255,100));
		al_draw_filled_rounded_rectangle(this->x1, this->y1,
			this->x2, this->y2,(x2-x1)/2 ,(y2-y1)/2*0.8 , al_map_rgba(255, 255, 255,100));
	}
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	//--------------------------------------
	int h = 0,w = 0;
	int x = 0,y = 0;
	FBResource::Inst().GetFont()->GetWidthHeight(text.c_str(),w,h);
	x = x1+ (x2-x1-w)/2;
	y = y1+ (y2-y1-h)/2;
	FBResource::Inst().GetFont()->Draw(x,y,this->text.c_str(),1);

	//--------------------------------------

	al_set_blender(op,src,dst);

	if(flash_time > 0)
		flash_time--;

	if(flash_time == 1)
	{
		clicked_flag = true;
	}
}


ToggleButton::ToggleButton(std::string text,bool toggle)
	:text(text),toggle(toggle)
{
}

void ToggleButton::set_toggle()
{
	this->toggle = !toggle;
}

bool ToggleButton::get_toggle()
{
	return this->toggle;
}

void ToggleButton::on_selected(int x, int y)
{
	(void)x;(void)y;
	if(selected == TOUCH_OFF)
		set_toggle();
}

void ToggleButton::draw()
{

	ALLEGRO_COLOR fg;
	ALLEGRO_COLOR bg;
	bg = al_map_rgba(255, 255, 255,100);
	fg = al_map_rgb(0, 0, 0);

	int op,src,dst;
	al_get_blender(&op,&src,&dst);

	if (this->selected == TOUCH_ON) 
	{
		al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_ALPHA);
		//al_draw_filled_rectangle(x,y,x+w,y+h,al_map_rgba(255, 255, 255,100));
		al_draw_filled_rounded_rectangle(this->x1, this->y1,
			this->x2, this->y2,(x2-x1)/2 ,(y2-y1)/2*0.8 , al_map_rgba(255, 255, 255,100));
	}
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

	//--------------------------------------
	int h = 0,w = 0;
	int x = 0,y = 0;
	FBResource::Inst().GetFont()->GetWidthHeight(text.c_str(),w,h);

	x = x1+ (x2-x1-w)/2;
	y = y1+ (y2-y1-h)/2;
	FBResource::Inst().GetFont()->Draw(x,y,this->text.c_str(),1);

	//--------------------------------------
	int rect_x = x + w + h/2;
	int rect_y = y;
	int rect_w = h;
	int rect_h = h;

	al_draw_rectangle(rect_x,rect_y,rect_x+rect_w,rect_y+rect_h, fg, 3);

	if (this->toggle)
	{
		int rect_w2 = rect_w/2;
		int rect_h2 = rect_h/2;
		int rect_x2 = rect_x + (rect_w-rect_w2)/2;
		int rect_y2 = rect_y + (rect_h-rect_h2)/2;

		al_draw_filled_rectangle(rect_x2,rect_y2,rect_x2+rect_w2,rect_y2+rect_h2,bg);
	}

	//--------------------------------------
	al_set_blender(op,src,dst);
}

UpDownControl::UpDownControl(std::string text, int value, int min, int max)
	:text(text),value(value),maximum(max),minimum(min),changed_flag(false),left_array_flag(true)
{

}

void UpDownControl::on_selected(int x, int y)
{
	(void)y;
	if(selected == TOUCH_ON)
	{
		if(x > x1 && x < (x1 + x2)/2)
		{
			left_array_flag = true;
			changed_flag = true;
			value--;
			if(value < minimum)
				value = minimum;
		}
		else
		{
			left_array_flag = false;
			changed_flag = true;
			value++;
			if(value > maximum)
				value = maximum;
		}

	}
}

bool UpDownControl::is_changed()
{
	bool r = changed_flag;
	changed_flag = false; 
	return r;
}

void UpDownControl::draw()
{
	ALLEGRO_COLOR fg;
	ALLEGRO_COLOR bg;
	SaveState state;

	if (selected == TOUCH_ON) 
	{
		bg = al_map_rgba(255, 255, 255,100);
		fg = al_map_rgb(0, 0, 0);
	}
	else
	{
		bg = al_map_rgba(255, 255, 255,100);
		fg = al_map_rgb(0, 0, 0);
	}

	int op,src,dst;

	al_get_blender(&op,&src,&dst);

	if (selected == TOUCH_ON && left_array_flag) 
	{
		al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_ALPHA);
		//al_draw_filled_rectangle(x,y,x+w,y+h,al_map_rgba(255, 255, 255,100));
		al_draw_filled_rounded_rectangle(this->x1, this->y1,
			(x2+x1)/2, this->y2,((x2+x1)/2-x1)/2 ,(y2-y1)/2*0.8 , al_map_rgba(255, 255, 255,100));
	}
	else if(selected == TOUCH_ON && !left_array_flag)
	{
		al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_ALPHA);
		al_draw_filled_rounded_rectangle((x2+x1)/2, this->y1,
			this->x2, this->y2,(x2-(x2+x1)/2)/2 ,(y2-y1)/2*0.8 , al_map_rgba(255, 255, 255,100));        
	}

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	//--------------------------------------
	//draw text
	int h = 0,w = 0;
	int x = 0,y = 0;
	FBResource::Inst().GetFont()->GetWidthHeight(text.c_str(),w,h);

	char str[32] = {0};
	int i_h = 0,i_w = 0;
	int i_x = 0,i_y = 0;
	sprintf(str,"%d",value);
	FBResource::Inst().GetFont()->GetWidthHeight(str,i_w,i_h);

	x = x1+ (x2-x1-w-i_w-i_h)/2;
	y = y1+ (y2-y1-h)/2;
	FBResource::Inst().GetFont()->Draw(x,y,this->text.c_str(),1);
	//--------------------------------------
	//draw integer
	i_x = x + w + i_h;
	i_y = y;    
	FBResource::Inst().GetFont()->Draw(i_x,i_y,str,1);
	//--------------------------------------
	//draw arrow
	float p1_x,p1_y,p2_x,p2_y,p3_x,p3_y;
	p1_x = x1;
	p1_y = y+h/2;
	p2_x = x1+h;
	p2_y = y;
	p3_x = x1+h;
	p3_y = y+h;    
	al_draw_triangle(p1_x,p1_y,p2_x, p2_y, p3_x, p3_y, fg, 3);

	p1_x = x2;
	p1_y = y+h/2;
	p2_x = x2-h;
	p2_y = y;
	p3_x = x2-h;
	p3_y = y+h;    
	al_draw_triangle(p1_x,p1_y,p2_x, p2_y, p3_x, p3_y, fg, 3);

	//--------------------------------------

	al_set_blender(op,src,dst);

}

