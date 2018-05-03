#include "SplashResource.hpp"
#include "FBResource.hpp"


#define RELEASE_RES(x) if(x){delete x;x = NULL;}

#define LOAD_BITMAPN_RES(x,y,z,w,h,r,c)  	\
	if((x = y::LoadFrom(z,w,h,r,c)) == NULL)\
	{\
		return false;\
	}\


#define LOAD_RES(x,y,z)  	\
	if((x = y::LoadFrom(z)) == NULL)\
	{\
		return false;\
	}\

#define LOAD_FONT_RES(x,y,z,u,v)  	\
	if((x = BitmapFont::LoadFrom(y,z,u,v)) == NULL)\
	{\
		return false;\
	}\


Prop* SplashResource_::CreateAppFrozenBubble()
{
	return new Prop(pAppFrozenBubble,(640-280)/2+(280-128)/2,190+(156-128)/2,128,128);
}

Prop* SplashResource_::CreateVoidPanel()
{
	return new Prop(pVoidPanel,(640-280)/2,190,156,280);
}

PropFont* SplashResource_::CreateFont()
{
	return new PropFont(pFont,185,433,10,10);
}

void SplashResource_::Destroy(void)
{
	RELEASE_RES( pFont)
	RELEASE_RES( pVoidPanel)
	RELEASE_RES( pAppFrozenBubble)
}

bool SplashResource_::Load(void)
{
	LOAD_FONT_RES(pFont,RES_BUBBLE_FONT,RES_POSITION,RES_CHARACTERS, FONT_HEIGHT)
	LOAD_RES( pVoidPanel,Bitmap,RES_VOID_PANEL)
	LOAD_RES( pAppFrozenBubble,Bitmap,RES_APP_FROZEN_BUBBLE)

	return true;
}

