#include "PropWidget.hpp"

PropWidget::~PropWidget()
{


}

bool PropWidget::OnMouseUp(int x, int y)
{
	if(!current_widget)
		return false;

	current_widget->set_selected(TOUCH_OFF,x,y);

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
		current_widget->set_selected(TOUCH_MOVEOUT,x,y);
		current_widget = NULL;

		for(std::list<Widget*>::const_iterator i = all_widgets.begin(),
			m = all_widgets.end();i != m;i++)
		{
			if((*i)->contains(x, y))
			{
				current_widget = *i;
				(*i)->set_selected(TOUCH_ON,x,y);
			}
		}
	}
	return false;
}

bool PropWidget::OnMouseDown(int x, int y)
{
	for(std::list<Widget*>::const_iterator i = all_widgets.begin(),
		m = all_widgets.end();i != m;i++)
	{
		if((*i)->contains(x, y))
		{
			current_widget = *i;
			(*i)->set_selected(TOUCH_ON,x,y);
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
		x_padding = offsetx;
		y_padding = offsety;
		configure_all();

		int cx, cy, cw, ch;
		al_get_clipping_rectangle(&cx, &cy, &cw, &ch);

		for (std::list<Widget *>::iterator it = this->all_widgets.begin();
			it != this->all_widgets.end();
			++it)
		{
			Widget *wid = (*it);
			al_set_clipping_rectangle(wid->x1, wid->y1, wid->width(), wid->height());
			wid->draw();
		}

		al_set_clipping_rectangle(cx, cy, cw, ch);

		this->draw_requested = false;
	}
}

void PropWidget::configure_all()
{
   //const int xsize = al_get_display_width(display) / this->grid_m;
   //const int ysize = al_get_display_height(display) / this->grid_n;

   const int xsize = 1;
   const int ysize = 1;

   for (std::list<Widget*>::iterator it = this->all_widgets.begin();
         it != this->all_widgets.end();
         ++it)
   {
      (*it)->configure(xsize, ysize, this->x_padding, this->y_padding);
   }
}

void PropWidget::add(Widget* widget, int grid_x, int grid_y, int grid_w,
   int grid_h)
{
   widget->grid_x = grid_x;
   widget->grid_y = grid_y;
   widget->grid_w = grid_w;
   widget->grid_h = grid_h;
   widget->theme  = this->get_theme();
   widget->parent = this;

   this->all_widgets.push_back(widget);
}
