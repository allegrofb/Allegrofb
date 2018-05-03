#ifndef THEME_H
#define THEME_H

#include "Alo.hpp"

class Theme 
{
public:
   ALLEGRO_COLOR        bg;
   ALLEGRO_COLOR        fg;
   ALLEGRO_COLOR        highlight;
   ALLEGRO_FONT   *font;

   // Null font is fine if you don't use a widget that requires text.
   explicit Theme(ALLEGRO_FONT *p=NULL);
   ~Theme();
};



#endif	//THEME_H

