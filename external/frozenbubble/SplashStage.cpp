#include "SplashStage.hpp"
#include "FBResource.hpp"
#include "GlobalSetting.hpp"
#include "SplashResource.hpp"

int SplashStage::OnUpdate()
{
	myLoader->Start();

	return 1;
}

bool SplashStage::OnInit()
{
	if(!SplashResource::Inst().Load())
		return false;

	SetCenterOffset(GlobalSetting::Inst().StageOffsetX,GlobalSetting::Inst().StageOffsetY);

	Add(SplashResource::Inst().CreateVoidPanel());
	Add(SplashResource::Inst().CreateAppFrozenBubble());
	myLevelStr = SplashResource::Inst().CreateFont();Add(myLevelStr);
	myLevelStr->Assign("LOADING...");

	return true;
}

void SplashStage::OnRelease()
{
	SplashResource::Inst().Destroy();
	delete myLoader;
}




