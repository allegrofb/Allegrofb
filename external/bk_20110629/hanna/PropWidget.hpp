#ifndef PROPWIDGET_H
#define PROPWIDGET_H

#include "PropBase.hpp"
#include "Widget.hpp"
#include "Theme.hpp"

class PropWidget : public PropBase
{
public:
	PropWidget(Theme* p, int x,int y,int h,int w, bool v = true)
		:theme(p), PropBase(x,y,h,w,v), grid_m(20),grid_n(40),x_padding(1),y_padding(1)
	{
		current_widget = NULL;
	}
	~PropWidget();
	bool OnMouseUp(int x, int y);
	bool OnMouseMoving(int x, int y);
	bool OnMouseDown(int x, int y);
	void OnUpdate();
public:
	virtual void Render(int offsetx,int offsety,double zoom);
//private:
	Theme*				theme;
	int                  grid_m;
	int                  grid_n;
	int                  x_padding;
	int                  y_padding;

	bool                 draw_requested;
	bool                 quit_requested;
	std::list<Widget *>  all_widgets;
	Widget*              mouse_over_widget;
	Widget*              mouse_down_widget;
	Widget*              key_widget;
	Widget*				 current_widget;

public:
	void           set_padding(int x_padding, int y_padding);
	void           add(Widget* widget, int grid_x, int grid_y, int grid_w, int grid_h);
	void           prepare();
	void           run_step(bool block);
	void           request_quit();
	bool           is_quit_requested() const;
	void           request_draw();
	bool           is_draw_requested() const;
	Theme*		  get_theme() const {return theme;};

private:
	void           configure_all();
	void           on_key_down(const ALLEGRO_KEYBOARD_EVENT & event);
	void           on_mouse_axes(const ALLEGRO_MOUSE_EVENT & event);
	void           check_mouse_over(int mx, int my);
	void           on_mouse_button_down(const ALLEGRO_MOUSE_EVENT & event);
	void           on_mouse_button_up(const ALLEGRO_MOUSE_EVENT & event);
	void   on_mouse_button_down(int mx, int my) { (void)mx; (void)my; }
	void   on_mouse_button_hold(int mx, int my) { (void)mx; (void)my; }
	void   on_mouse_button_up(int mx, int my) { (void)mx; (void)my; }
};


#endif	//PROPWIDGET_H

