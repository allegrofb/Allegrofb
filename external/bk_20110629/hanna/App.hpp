#ifndef APP_H
#define APP_H

#include <vector>
#include "Stage.hpp"

class App
{
public:
	template<class Runner>
	static int Running(Runner app){return app.Run();};
	void Add(Stage* p);
protected:
	int Run();
	virtual bool OnInit(){return true;};
	virtual void OnRelease(){};
private:	
	bool Init();
	void Release();
private:
	ALLEGRO_DISPLAY *myDisplay;
	std::vector<Stage*> stageVector;
};


#endif	//APP_H

