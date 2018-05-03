#ifndef SETTING_HPP
#define SETTING_HPP

#include "ResourceManager.hpp"

class Setting_ : public ResourceManager
{
public:
    Setting_(int frameRate, int stageWidth, int stageHeight, 
             int displayWidth, int displayHeight, int stageOffsetX, 
             int stageOffsetY, double stageZoom)
    {
        FrameRate       = frameRate;
        StageWidth      = stageWidth;
        StageHeight     = stageHeight;
        DisplayWidth    = displayWidth;
        DisplayHeight   = displayHeight;
        StageOffsetX    = stageOffsetX;
        StageOffsetY    = stageOffsetY;
        StageZoom       = stageZoom;
    }
    int  FrameRate;
    int  StageWidth;
    int  StageHeight;
    int  DisplayWidth;
    int  DisplayHeight;
    int  StageOffsetX;
    int  StageOffsetY;
    double StageZoom;
    
public:
	virtual void Destroy(void){}
	virtual bool Load(void){return true;}
    
    int  GetFrame(double time)
    {
        return (int)(time*FrameRate);
    }
    double GetTime(int frame)
    {
        return (double)frame/(double)FrameRate;
    }    
};



#endif // SETTING_HPP

