#include "PropWidget.hpp"

PropWidget::~PropWidget()
{

}

bool PropWidget::OnMouseUp(int x, int y)
{
	if(!current_widget)
		return false;

	current_widget->set_selected(Widget::TOUCH_OFF,x,y);

	current_widget = NULL;
	return false;
}

bool PropWidget::OnMouseMoving(int x, int y)
{
	if(!current_widget)
		return false;

	if(current_widget->contains(x,y))
	{
		//current_widget
	}
	else
	{
		current_widget->set_selected(Widget::TOUCH_MOVEOUT,x,y);
		current_widget = NULL;

		for(std::list<Widget*>::const_iterator i = begin(),
			m = end();i != m;i++)
		{
			if((*i)->contains(x, y))
			{
				current_widget = *i;
				(*i)->set_selected(Widget::TOUCH_ON,x,y);
			}
		}
	}
	return false;
}

bool PropWidget::OnMouseDown(int x, int y)
{
	for(std::list<Widget*>::const_iterator i = begin(),
		m = end();i != m;i++)
	{
		if((*i)->contains(x, y))
		{
			current_widget = *i;
			(*i)->set_selected(Widget::TOUCH_ON,x,y);
		}
	}

	return false;
}

void PropWidget::OnUpdate()
{

}

void PropWidget::Render(int offsetx,int offsety,double zoom)
{
	if(myDelay)
	{
		myDelay--;
		return;
	}

	if(myVisible)
	{
		configure_all(offsetx,offsety,zoom);

		int cx, cy, cw, ch;
		al_get_clipping_rectangle(&cx, &cy, &cw, &ch);

		for (std::list<Widget *>::iterator it = begin();
			it != end();++it)
		{
			Widget *wid = (*it);
            wid->set_clippling_rectangle();
			wid->draw();
		}

		al_set_clipping_rectangle(cx, cy, cw, ch);
	}
}

void PropWidget::configure_all(int offsetx,int offsety,double zoom)
{
   for (std::list<Widget*>::iterator it = begin();
         it != end();++it)
   {
       (*it)->configure(offsetx,offsety,zoom);
   }
}

void PropWidget::Add(Widget* widget, int grid_x, int grid_y, int grid_w,
   int grid_h)
{
    widget->set_xy_wh(grid_x,grid_y,grid_w,grid_h);    
    push_back(widget);
}
