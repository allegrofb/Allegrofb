#ifndef STAGE_H
#define STAGE_H

#include "Alo.hpp"
#include "Prop.hpp"
#include <vector>

class Stage
{
public:
	int Run(ALLEGRO_DISPLAY *display);
	void Close(int code=-1);
	void AssignTrans(ALLEGRO_TRANSFORM& t);
protected:
	Stage();
	void Draw(Drawable* p);	//draw with stage coordinary 
	void Add(Drawable* p);
	void UpdateCenterOffset(int& x, int& y);
	void SetCenterOffset(int x, int y);	//update stage center in the display
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
	void Render();
private:
	ALLEGRO_EVENT_QUEUE *myQueue;
    ALLEGRO_TRANSFORM myTransform;
    ALLEGRO_TRANSFORM myTransformInverse;
	ALLEGRO_BITMAP *myBuffer;
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

