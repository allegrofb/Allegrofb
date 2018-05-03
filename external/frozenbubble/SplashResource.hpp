#ifndef SPLASHRESOURCE_HPP
#define SPLASHRESOURCE_HPP

#include "hanna/ResourceManager.hpp"
#include "hanna/Bitmap.hpp"
#include "hanna/BitmapFont.hpp"
#include "hanna/Prop.hpp"
#include "hanna/PropFont.hpp"
#include "hanna/Singleton.hpp"

class SplashResource_ : public ResourceManager
{
public:
	virtual void Destroy(void);
	virtual bool Load(void);
public:
	Prop* CreateVoidPanel();
	PropFont* CreateFont();
	Prop* CreateAppFrozenBubble();
private:
	BitmapFont* pFont;
	Bitmap* pVoidPanel;
	Bitmap* pAppFrozenBubble;
};

typedef singleton_default<SplashResource_> SplashResource;


#endif // SPLASHRESOURCE_HPP

