#include "Widget.hpp"

#include <iostream>
#include <string>
#include <algorithm>

#include "../FBResource.hpp"


#define CLAMP(x,y,z) (std::max)(x, (std::min)(y, z))

namespace {

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

/*---------------------------------------------------------------------------*/

Widget::Widget():
grid_x(0),
	grid_y(0),
	grid_w(0),
	grid_h(0),
	x1(0),
	y1(0),
	x2(0),
	y2(0),
	selected(TOUCH_OFF)
{
}

void Widget::configure(int xsize, int ysize, int x_padding, int y_padding)
{
	//this->x1 = xsize * this->grid_x + x_padding;
	//this->y1 = ysize * this->grid_y + y_padding;
	//this->x2 = xsize * (this->grid_x + this->grid_w) - x_padding - 1;
	//this->y2 = ysize * (this->grid_y + this->grid_h) - y_padding - 1;
	this->x1 = this->grid_x + x_padding;
	this->y1 = this->grid_y + y_padding;
	this->x2 = (this->grid_x + this->grid_w) + x_padding;
	this->y2 = (this->grid_y + this->grid_h) + y_padding;
}

bool Widget::contains(int x, int y)
{
	return (x >= this->x1 && y >= this->y1 && x <= this->x2 && y <= this->y2);
}

/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/

Label::Label(std::string text, bool centred) :
text(text),
	centred(centred)
{
}

void Label::draw()
{
	SaveState state;

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	if (centred) {
		al_draw_text(theme->font, theme->fg, (this->x1 + this->x2 + 1)/2,
			this->y1, ALLEGRO_ALIGN_CENTRE, this->text.c_str());
	}
	else {
		al_draw_text(theme->font, theme->fg, this->x1, this->y1, 0, this->text.c_str());
	}
}

void Label::set_text(std::string new_text)
{
	this->text = new_text;
}

bool Label::want_mouse_focus()
{
	return false;
}

/*---------------------------------------------------------------------------*/

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

	if (this->flash_time) {
		fg = theme->bg;
		bg = theme->fg;
	}
	else {
		fg = theme->fg;
		bg = theme->bg;
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

/*---------------------------------------------------------------------------*/

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
	fg = theme->fg;
	bg = theme->bg;


	int op,src,dst;
	al_get_blender(&op,&src,&dst);

	if (this->selected == TOUCH_ON) 
	{
		al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_ALPHA);
		//al_draw_filled_rectangle(x,y,x+w,y+h,al_map_rgba(255, 255, 255,100));
		al_draw_filled_rounded_rectangle(this->x1, this->y1,
			this->x2, this->y2,(x2-x1)/2 ,(y2-y1)/2*0.8 , al_map_rgba(255, 255, 255,100));
	}
	//al_draw_filled_rounded_rectangle(this->x1, this->y1,
	//   this->x2, this->y2,(x2-x1)/2*0.8 ,(y2-y1)/2*0.7 , al_map_rgba(255, 255, 255,100));
	//al_draw_rounded_rectangle(this->x1, this->y1,
	//   this->x2, this->y2,(x2-x1)/2*0.8 ,(y2-y1)/2*0.7 , al_map_rgba(255, 255, 255,0),3);



	//al_draw_filled_rectangle(this->x1, this->y1,
	//   this->x2, this->y2, bg);
	//al_draw_rectangle(this->x1 + 0.5, this->y1 + 0.5,
	//   this->x2 - 0.5, this->y2 - 0.5, fg, 0);

	//1. transparent text
	//default
	//2. black background text
	//al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ZERO);
	//3. black background text, src background 255????
	//al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
	//4. transparent text 
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	//5. transparent text
	//al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	//6. transparent text with black color 
	//al_set_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ONE, ALLEGRO_ONE);


	//al_draw_text(theme->font, fg, (this->x1 + this->x2 + 1)/2,
	//   this->y1, ALLEGRO_ALIGN_CENTRE, this->text.c_str());

	//hyjiang for test
	//FBResource::Inst().GetFont()->Draw(this->x1,this->y1,this->text.c_str());

	//al_draw_justified_text(theme->font, fg, x1, x2, 50, diff,
	//   0, text_entry.get_text());

	//al_draw_justified_text(theme->font, fg, (this->x1 + this->x2 + 1)/2,
	//   this->y1,50,100, 0, this->text.c_str());



	//--------------------------------------
	int h = 0,w = 0;
	int x = 0,y = 0;
	FBResource::Inst().GetFont()->GetWidthHeight(text.c_str(),w,h);
	//int w2 = w+ h+h/2;

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

/*---------------------------------------------------------------------------*/

UpDownControl::UpDownControl(std::string text, int value, int min, int max)
    :text(text),changed_flag(false),value(value),left_array_flag(true),maximum(max),minimum(min)
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
    
	if (selected == TOUCH_ON) {
		fg = theme->bg;
		bg = theme->fg;
	}
	else {
		fg = theme->fg;
		bg = theme->bg;
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

/*---------------------------------------------------------------------------*/
const std::string List::empty_string;

List::List(int initial_selection) :
selected_item(initial_selection)
{
}

bool List::want_key_focus()
{
	return true;
}

void List::on_key_down(const ALLEGRO_KEYBOARD_EVENT & event)
{
	switch (event.keycode) {
	case ALLEGRO_KEY_DOWN:
		if (selected_item < items.size() - 1) {
			selected_item++;

		}
		break;

	case ALLEGRO_KEY_UP:
		if (selected_item > 0) {
			selected_item--;

		}
		break;
	}
}

void List::on_click(int mx, int my)
{
	unsigned int i = (my - this->y1) / al_get_font_line_height(theme->font);
	if (i < this->items.size()) {
		this->selected_item = i;

	}

	(void)mx;
	(void)my;
}

void List::draw()
{
	SaveState state;

	al_draw_filled_rectangle(x1 + 1, y1 + 1, x2 - 1, y2 - 1, theme->bg);

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	const int font_height = al_get_font_line_height(theme->font);
	for (unsigned i = 0; i < items.size(); i++) {
		int yi = y1 + i * font_height;

		if (i == selected_item) {
			al_draw_filled_rectangle(x1 + 1, yi, x2 - 1, yi + font_height - 1,
				theme->highlight);
		}

		al_draw_text(theme->font, theme->fg, x1, yi, 0, items.at(i).c_str());
	}
}

void List::clear_items()
{
	this->items.clear();
	this->selected_item = 0;
}

void List::append_item(std::string text)
{
	this->items.push_back(text);
}

const std::string & List::get_selected_item_text() const
{
	if (this->selected_item < this->items.size())
		return this->items.at(this->selected_item);
	else
		return List::empty_string;
}

int List::get_cur_value() const
{
	return this->selected_item;
}

/*---------------------------------------------------------------------------*/

VSlider::VSlider(int cur_value, int max_value) :
cur_value(cur_value),
	max_value(max_value)
{
}

void VSlider::on_mouse_button_down(int mx, int my)
{
	this->on_mouse_button_hold(mx, my);
}

void VSlider::on_mouse_button_hold(int mx, int my)
{
	double r = (double) (this->y2 - 1 - my) / (this->height() - 2);
	r = CLAMP(0.0, r, 1.0);
	cur_value = (int) (r * max_value);


	(void)mx;
}

void VSlider::draw()
{
	float left = x1 + 0.5, top = y1 + 0.5;
	float right = x2 + 0.5, bottom = y2 + 0.5;
	SaveState state;

	al_draw_rectangle(left, top, right, bottom, theme->fg, 1);

	double ratio = (double) this->cur_value / (double) this->max_value;
	int ypos = (int) (bottom - 0.5 - (int) (ratio * (height() - 7)));
	al_draw_filled_rectangle(left + 0.5, ypos - 5, right - 0.5, ypos, theme->fg);
}

int VSlider::get_cur_value() const
{
	return this->cur_value;
}

void VSlider::set_cur_value(int v)
{
	this->cur_value = v;
}

/*---------------------------------------------------------------------------*/

HSlider::HSlider(int cur_value, int max_value) :
cur_value(cur_value),
	max_value(max_value)
{
}

void HSlider::on_mouse_button_down(int mx, int my)
{
	this->on_mouse_button_hold(mx, my);
}

void HSlider::on_mouse_button_hold(int mx, int my)
{
	double r = (double) (mx - 1 - this->x1) / (this->width() - 2);
	r = CLAMP(0.0, r, 1.0);
	cur_value = (int) (r * max_value);


	(void)my;
}

void HSlider::draw()
{
	const int cy = (y1 + y2) / 2;
	SaveState state;

	al_draw_filled_rectangle(x1, y1, x2, y2, theme->bg);
	al_draw_line(x1, cy, x2, cy, theme->fg, 0);

	double ratio = (double) this->cur_value / (double) this->max_value;
	int xpos = x1 + (int) (ratio * (width() - 2));
	al_draw_filled_rectangle(xpos - 2, y1, xpos + 2, y2, theme->fg);
}

int HSlider::get_cur_value() const
{
	return this->cur_value;
}

void HSlider::set_cur_value(int v)
{
	this->cur_value = v;
}

/*---------------------------------------------------------------------------*/

TextEntry::TextEntry(const char *initial_text) :
focused(false),
	cursor_pos(0),
	left_pos(0)
{
	text = al_ustr_new(initial_text);
}

TextEntry::~TextEntry()
{
	al_ustr_free(text);
}

bool TextEntry::want_key_focus()
{
	return true;
}

void TextEntry::got_key_focus()
{
	this->focused = true;

}

void TextEntry::lost_key_focus()
{
	this->focused = false;

}

void TextEntry::on_key_down(const ALLEGRO_KEYBOARD_EVENT & event)
{
	switch (event.keycode) {
	case ALLEGRO_KEY_LEFT:
		al_ustr_prev(text, &cursor_pos);
		break;

	case ALLEGRO_KEY_RIGHT:
		al_ustr_next(text, &cursor_pos);
		break;

	case ALLEGRO_KEY_HOME:
		cursor_pos = 0;
		break;

	case ALLEGRO_KEY_END:
		cursor_pos = al_ustr_size(text);
		break;

	case ALLEGRO_KEY_DELETE:
		al_ustr_remove_chr(text, cursor_pos);
		break;

	case ALLEGRO_KEY_BACKSPACE:
		if (al_ustr_prev(text, &cursor_pos))
			al_ustr_remove_chr(text, cursor_pos);
		break;

	default:
		if (event.unichar >= ' ') {
			al_ustr_insert_chr(text, cursor_pos, event.unichar);
			cursor_pos += al_utf8_width(event.unichar);
		}
		break;
	}

	maybe_scroll();

}

void TextEntry::maybe_scroll()
{

	if (cursor_pos < left_pos + 3) {
		if (cursor_pos < 3)
			left_pos = 0;
		else
			left_pos = cursor_pos - 3;
	}
	else {
		for (;;) {
			const int tw = al_get_ustr_width(theme->font,
				UString(text, left_pos, cursor_pos));
			if (x1 + tw + CURSOR_WIDTH < x2) {
				break;
			}
			al_ustr_next(text, &left_pos);
		}
	}
}

void TextEntry::draw()
{
	SaveState state;

	al_draw_filled_rectangle(x1, y1, x2, y2, theme->bg);

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

	if (!focused) {
		al_draw_ustr(theme->font, theme->fg, x1, y1, 0, UString(text, left_pos));
	}
	else {
		int x = x1;

		if (cursor_pos > 0) {
			UString sub(text, left_pos, cursor_pos);
			al_draw_ustr(theme->font, theme->fg, x1, y1, 0, sub);
			x += al_get_ustr_width(theme->font, sub);
		}

		if ((unsigned) cursor_pos == al_ustr_size(text)) {
			al_draw_filled_rectangle(x, y1, x + CURSOR_WIDTH,
				y1 + al_get_font_line_height(theme->font), theme->fg);
		}
		else {
			int post_cursor = cursor_pos;
			al_ustr_next(text, &post_cursor);

			UString sub(text, cursor_pos, post_cursor);
			int subw = al_get_ustr_width(theme->font, sub);
			al_draw_filled_rectangle(x, y1, x + subw,
				y1 + al_get_font_line_height(theme->font), theme->fg);
			al_draw_ustr(theme->font, theme->bg, x, y1, 0, sub);
			x += subw;

			al_draw_ustr(theme->font, theme->fg, x, y1, 0,
				UString(text, post_cursor));
		}
	}
}

const char *TextEntry::get_text()
{
	return al_cstr(text);
}


