#ifndef STAGE_H
#define STAGE_H

#include "Alo.hpp"
#include "Prop.hpp"
#include <vector>
#include "App.hpp"

class Stage : public Drawable
{
public:
	Stage();
    Setting_* setting;
public:
	int Run(ALLEGRO_DISPLAY *display);
protected:
	void Add(Drawable* p);
	void GetCenterOffset(int& x, int& y);
	void SetCenterOffset(int x, int y);	//update stage center in the display
	void Close(int code=-1);
	void AssignTrans(ALLEGRO_TRANSFORM& t);
protected:
	virtual void Render(int x, int y, double zoom);
	virtual void Render();
	virtual bool OnInit(){return true;};
	virtual int OnUpdate(){return 0;};
	virtual void OnRelease(){};
	virtual void OnChar(int keycode, int unichar){(void)keycode;(void)unichar;};
	virtual bool OnMouseDown(int x, int y){(void)x;(void)y;return false;};
	virtual bool OnMouseMoving(int x, int y){(void)x;(void)y;return false;};
	virtual bool OnMouseUp(int x, int y){(void)x;(void)y;return false;};
	virtual void OnBackGround(){};
	virtual void OnForeGround(){};
    virtual void OnOrientation(int orientation){(void)orientation;};
	virtual void OnShakeGesture(){};
private:
	void Release();
	bool Init();
    App* myApp;
private:
	ALLEGRO_EVENT_QUEUE *myQueue;
    ALLEGRO_TRANSFORM myTransform;
	int myHeight;	//stage's resolution
	int myWidth;
	int myCenterOffsetX;	//stage center offset according to display
	int myCenterOffsetY;	//stage center offset according to display
	int myDisplayHeight;	//display's resolution
	int myDisplayWidth;
	int myZoom;

	std::vector<Drawable*> drawableList;
	int myExitCode;
	bool myMouseButtonDown;
};

#endif	//STAGE_H

