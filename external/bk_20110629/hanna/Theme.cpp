#include "Theme.hpp"


Theme::Theme(ALLEGRO_FONT *p)
{
	this->bg = al_map_rgba(255, 255, 255,100);
	this->fg = al_map_rgb(0, 0, 0);
	this->highlight = al_map_rgb(128, 128, 255);
	this->font = p;
}

Theme::~Theme()
{
	if(font)   
		al_destroy_font(font);
};



