#ifndef FBBUBBLE_H
#define FBBUBBLE_H

#include "hanna/Bitmap.hpp"
#include "hanna/Animation.hpp"
#include "hanna/Drawable.hpp"
#include "hanna/BitmapArray.hpp"

#define UPDATE_DEFAULT          0
#define UPDATE_ONEBUBBLE_FIXED  1
#define UPDATE_FALLING          2
#define UPDATE_FAILED           3

class FBBubble : public Drawable
{
public:
	FBBubble(BitmapArray* p, BitmapArray* pFixed,BitmapArray* pFrozen,Bitmap* pBlink);
	~FBBubble();
	virtual void Render(int x, int y,double zoom);
	int Update();	//0 - default, 1 - one bubble fixed, 2 - falling, 3 - fail
	void MoveDown();
	bool CheckFail();
	bool CheckSuccess();
	void Fire(float angle);
	void Fire(int x, int y, float& angle);
    void CalculateAngle(int x, int y, float& angle);    
	bool MovingBubbleExist();
	void SetFail(){endOfGame = 1;};
	void SetBlink(int num){blinkNum = num;};
	void InitLevel(int i);
	void SetMovingOnesInvalid();
private:
	void ClearMovingOnes();
	void SetLevel(int i);	//1,2,3 ...
private:
	struct Data
	{
		Data():type(0),l(0),t(0),w(0),h(0),id(0),state(0),fixed(0){}
		int type; //different color
		int l;
		int t;
		int w;
		int h;

		int fixed;	//for fixed animation
		int id;		//for jumpping and falling calculation 
		int state;	//0 - default, 1 - moving, 2 - jumpping, 3 - falling
		float angle;//moving angle
		int jumpingx;
		int jumpingy;
		int fallingy;
		int tick;
	};

	Data myData[8][13];
	Data nextOne;
	Data launchOne;
	BitmapArray* bitmap;
	BitmapArray* fixedAnimation;
	BitmapArray* frozen;
	Bitmap*		 blinkBitmap;
	int compressorPos;
	int blinkNum;
	int endOfGame;

	struct Pos
	{
		Pos():row(-1),col(-1){}
		int row;
		int col;
	};
	std::vector<Data> movingOnes;
private:
	Pos GetCurrentPos(Data& data);
	void GetCurrentLeftTop(int row,int col, int &left, int &top);
	void GetNeighbors(Pos cur, Pos pos[]);
	bool CheckCollision(int x,int y,Pos pos[]);
	void GetJumps(int id,Pos cur,std::vector<Pos>& posList,Pos neighbors[]);
	void GenerateNextOne();
	void CheckFalls(int id,Pos cur);
	int myId;
	Pos lastPos;	////test purpose

	

};

#endif	//FBBUBBLE_H

