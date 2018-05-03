#ifndef FBBUBBLE_H
#define FBBUBBLE_H

#include "hanna/Drawable.hpp"
#include "hanna/Bitmap.hpp"
#include "hanna/BitmapArray.hpp"

class FBBubble : public Drawable
{
public:    
	FBBubble(BitmapArray* p, BitmapArray* pFixed,BitmapArray* pFrozen,Bitmap* pBlink);
	~FBBubble();    
	virtual void Render(int x, int y,double zoom);
public:    
    enum
    {
        UPDATE_DEFAULT = 0,
        UPDATE_ONEBUBBLE_FIXED,
        UPDATE_FALLING,
        UPDATE_FAILED,
    };    
	int Update();	
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
	struct Data
	{
		Data():type(0),l(0),t(0),w(0),h(0),id(0),state(DEFAULT),fixed(0){}
		int type; //different color
		int l;
		int t;
		int w;
		int h;

        enum STATE
        {
            DEFAULT = 0,
            MOVEING,
            JUMPING,
            FALLING,
        };
        
		int id;		//for jumpping and falling calculation 
		STATE state;	
		int fixed;	//for fixed animation
		float angle;//moving angle
		int jumpingx;
		int jumpingy;
		int fallingy;
		int tick;
	};
    
	struct Pos
	{
		Pos():row(-1),col(-1){}
		int row;
		int col;
	};

	Data myData[8][13];
	Data nextOne;
	Data launchOne;
	std::vector<Data> movingOnes;
	BitmapArray* bitmap;
	BitmapArray* fixedAnimation;
	BitmapArray* frozen;
	Bitmap*		 blinkBitmap;
	int compressorPos;
	int blinkNum;
	int endOfGame;
	int myId;    
private:
	void ClearMovingOnes();
	void SetLevel(int i);	//1,2,3 ...    
	Pos GetCurrentPos(Data& data);
	void GetCurrentLeftTop(int row,int col, int &left, int &top);
	void GetNeighbors(Pos cur, Pos pos[]);
	bool CheckCollision(int x,int y,Pos pos[]);
	void GetJumps(int id,Pos cur,std::vector<Pos>& posList,Pos neighbors[]);
	void GenerateNextOne();
	void CheckFalls(int id,Pos cur);
};

#endif	//FBBUBBLE_H

