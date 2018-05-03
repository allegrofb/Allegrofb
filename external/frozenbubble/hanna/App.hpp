#ifndef APP_H
#define APP_H

#include <vector>
#include "Setting.hpp"
#include "Alo.hpp"

class Stage;

class App
{
public:
    template<class Runner>
    static int Running(Setting_* p, Runner r)
    {
        return r.Run(p);
    }
protected:
	void Add(Stage* p);
	virtual bool OnInit(){return true;};
	virtual void OnRelease(){};
private:	
	int Run(Setting_* p);
	bool Init();
	void Release();
private:
	ALLEGRO_DISPLAY *myDisplay;
	std::vector<Stage*> stageVector;
    Setting_* setting;
};


#endif	//APP_H

