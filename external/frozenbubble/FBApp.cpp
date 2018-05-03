#include "FBApp.hpp"
#include "FBStage.hpp"
#include "FBResource.hpp"
#include "EntryStage.hpp"
#include "SplashStage.hpp"

bool FBApp::OnInit()
{
	//problem, two thread handle bitmap ?????
	Add(new SplashStage(new AsynLoader(&(FBResource::Inst()))));
	Add(new EntryStage());
	Add(new FBStage());
	return true;
}

void FBApp::OnRelease()
{
	FBResource::Inst().Destroy();
}


