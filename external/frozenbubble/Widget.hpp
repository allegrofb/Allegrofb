#ifndef WIDGET_H
#define WIDGET_H

#include <list>
#include <string>
#include <vector>
#include "hanna/PropBase.hpp"
#include "hanna/Alo.hpp"

class Widget
{
private:
	int      grid_x;
	int      grid_y;
	int      grid_w;
	int      grid_h;    
protected:
	int      x1;
	int      y1;
	int      x2;
	int      y2;
public:
    enum
    {
        TOUCH_OFF,
        TOUCH_ON,		
        TOUCH_MOVEOUT,	        
    };
    
	int            id;
	int            selected;
	unsigned int   width()  { return x2 - x1 + 1; }
	unsigned int   height() { return y2 - y1 + 1; }
	void           set_selected(int flag,int x,int y){selected = flag;on_selected(x,y);};
    void           set_xy_wh(int x,int y,int w,int h);
    void           set_clippling_rectangle();
	void           configure(int offsetx, int offsety, double zoom);
	bool           contains(int x, int y);
public:
	Widget();
	virtual ~Widget() {}

	virtual void   draw() = 0;
	virtual void   on_selected(int x,int y){(void)x;(void)y;};
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
};

#endif	//WIDGET_H

