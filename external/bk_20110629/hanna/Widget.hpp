#ifndef WIDGET_H
#define WIDGET_H

#include <list>
#include <string>
#include <vector>
#include "PropBase.hpp"
#include "Alo.hpp"
#include "Theme.hpp"

class PropWidget;

#define	TOUCH_OFF		0
#define	TOUCH_ON		1
#define TOUCH_MOVEOUT	2

class Widget
{
	friend class PropWidget;
private:
	int      grid_x;
	int      grid_y;
	int      grid_w;
	int      grid_h;

protected:
	PropWidget* parent;
	Theme*	    theme;
	int      x1;
	int      y1;
	int      x2;
	int      y2;
public:
	int id;
	int selected;
	void set_selected(int flag,int x,int y){selected = flag;on_selected(x,y);};
	virtual void on_selected(int x,int y){(void)x;(void)y;};
public:
	Widget();
	virtual ~Widget() {}

	void           configure(int xsize, int ysize, int x_padding, int y_padding);
	virtual bool   contains(int x, int y);
	unsigned int   width()  { return x2 - x1 + 1; }
	unsigned int   height() { return y2 - y1 + 1; }

	virtual bool   want_mouse_focus() { return true; }
	virtual void   got_mouse_focus() {}
	virtual void   lost_mouse_focus() {}
	virtual void   on_mouse_button_down(int mx, int my) { (void)mx; (void)my; }
	virtual void   on_mouse_button_hold(int mx, int my) { (void)mx; (void)my; }
	virtual void   on_mouse_button_up(int mx, int my) { (void)mx; (void)my; }
	virtual void   on_click(int mx, int my) { (void)mx; (void)my; }

	virtual bool   want_key_focus() { return false; }
	virtual void   got_key_focus() {}
	virtual void   lost_key_focus() {}
	virtual void   on_key_down(const ALLEGRO_KEYBOARD_EVENT & event) {(void)event; }

	virtual void   draw() = 0;

};


/*---------------------------------------------------------------------------*/
class Button : public Widget 
{
protected:
	std::string text;
	int         flash_time;
	bool        clicked_flag;
	enum
	{ 
		DELAY_TIME = 10
	};
public:
	explicit Button(std::string text);
	virtual void draw();
	virtual void on_selected(int x, int y);
	bool         is_clicked();
};

class ToggleButton : public Widget 
{
private:
	std::string text;
	bool        toggle;
public:
	explicit ToggleButton(std::string text, bool toggle=false);
	virtual void draw();
	virtual void on_selected(int x, int y);
	void         set_toggle();
	bool         get_toggle();
};


class UpDownControl : public Widget 
{
protected:
	std::string text;
    int         value;
    int         maximum;
    int         minimum;
	bool        changed_flag;
    bool        left_array_flag;
	enum
	{ 
		DELAY_TIME = 10
	};
public:
	explicit UpDownControl(std::string text, int value, int min, int max);
	virtual void draw();
	virtual void on_selected(int x, int y);
	bool         is_changed();
    int          get_value(){return value;};
};


class Label : public Widget 
{
private:
	std::string    text;
	bool           centred;
    
public:
	Label(std::string text="", bool centred=true);
	void set_text(std::string text);
	virtual void   draw();
	virtual bool   want_mouse_focus();
};


class List : public Widget {
private:
	static const   std::string empty_string;

	std::vector<std::string> items;
	unsigned int   selected_item;

public:
	List(int initial_selection = 0);
	virtual bool   want_key_focus();
	virtual void   on_key_down(const ALLEGRO_KEYBOARD_EVENT & event);
	virtual void   on_click(int mx, int my);
	virtual void   draw();

	void           clear_items();
	void           append_item(std::string text);
	const std::string & get_selected_item_text() const;
	int            get_cur_value() const;
};

class VSlider : public Widget {
private:
	int   cur_value;
	int   max_value;

public:
	VSlider(int cur_value = 0, int max_value = 1);

	virtual void   on_mouse_button_down(int mx, int my);
	virtual void   on_mouse_button_hold(int mx, int my);
	virtual void   draw();

	int            get_cur_value() const;
	void           set_cur_value(int v);
};

class HSlider : public Widget {
private:
	int   cur_value;
	int   max_value;

public:
	HSlider(int cur_value = 0, int max_value = 1);

	virtual void   on_mouse_button_down(int mx, int my);
	virtual void   on_mouse_button_hold(int mx, int my);
	virtual void   draw();

	int            get_cur_value() const;
	void           set_cur_value(int v);
};

class TextEntry : public Widget {
private:
	static const int CURSOR_WIDTH = 8;

	ALLEGRO_USTR   *text;
	bool           focused;
	int            cursor_pos;
	int            left_pos;

public:
	explicit TextEntry(const char *initial_text="");
	~TextEntry();

	virtual bool   want_key_focus();
	virtual void   got_key_focus();
	virtual void   lost_key_focus();
	virtual void   on_key_down(const ALLEGRO_KEYBOARD_EVENT & event);
	virtual void   draw();

	const char *   get_text();

private:
	void maybe_scroll();
};






#endif	//WIDGET_H

