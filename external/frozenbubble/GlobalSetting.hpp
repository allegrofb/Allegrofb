#ifndef GLOBALSETTING_HPP
#define GLOBALSETTING_HPP

#include "hanna/Setting.hpp"
#include "hanna/Singleton.hpp"

class GlobalSetting_ : public Setting_
{
public:
	virtual void Destroy(void);
	virtual bool Load(void);
    void Save(void);
    void Restore(void);
public:
	GlobalSetting_():Setting_(30,640,480,320,480,-160,0,1),MusicOn(true),SoundOn(true),LevelNum(1)
	{}
	bool MusicOn;
	bool SoundOn;
	int  LevelNum;
};

typedef singleton_default<GlobalSetting_> GlobalSetting;

#endif // GLOBALSETTING_HPP

