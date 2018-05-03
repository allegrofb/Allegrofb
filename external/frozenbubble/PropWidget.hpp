#ifndef PROPWIDGET_H
#define PROPWIDGET_H

#include "hanna/PropBase.hpp"
#include "Widget.hpp"

class PropWidget : public PropBase, public std::list<Widget* >
{
public:
	PropWidget(int x,int y,int h,int w, bool v = true)
		:PropBase(x,y,h,w,v)
	{
		current_widget = NULL;
	}
	~PropWidget();
	virtual void Render(int offsetx,int offsety,double zoom);
    
	bool OnMouseUp(int x, int y);
	bool OnMouseMoving(int x, int y);
	bool OnMouseDown(int x, int y);
	void OnUpdate();
	void Add(Widget* widget, int grid_x, int grid_y, int grid_w, int grid_h);
public:
//	bool    draw_requested;
	Widget* current_widget;
private:
	void configure_all(int offsetx,int offsety,double zoom);
};


#endif	//PROPWIDGET_H

