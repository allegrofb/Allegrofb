#include <math.h>
#include "FBBubble.hpp"
#include "FBResource.hpp"
#include "GlobalSetting.hpp"

FBBubble::~FBBubble()
{
}

void FBBubble::InitLevel(int i)
{
	compressorPos = 0;
	blinkNum = 0;
	endOfGame = 0;

	ClearMovingOnes();
	SetLevel(i);
}

void FBBubble::SetMovingOnesInvalid()
{
	for (std::vector<Data>::iterator i = movingOnes.begin();
		i != movingOnes.end();i++) 
	{
		if(i->state == Data::MOVEING)
		{
			i->state = Data::DEFAULT;
			i->type = 0;
		}
	}
}


void FBBubble::ClearMovingOnes()
{
	for (std::vector<Data>::iterator i = movingOnes.begin();
		i != movingOnes.end();i++) 
	{
		i->state = Data::DEFAULT;
		i->type = 0;
	}
}

void FBBubble::SetLevel(int i)
{
	const int* level = FBResource::Inst().Levels->GetLevel(i);

	memset(myData,0,sizeof(Data)*13*8);
	memset(dataNum,0,sizeof(int)*9);

	if(level)
	{
		for (int j=0 ; j<13 ; j++) 
		{
			for (int i=j%2 ; i<8 ; i++) 
			{
				if (level[i+j*8] >= 1 && level[i+j*8] <= 8) 
				{
					(myData[i][j]).type = level[j*8+i];
					dataNum[level[j*8+i]]++;
					(myData[i][j]).l = 190+i*32-(j%2)*16;
					(myData[i][j]).t = 44+j*28;
					(myData[i][j]).h = 32;
					(myData[i][j]).w = 32;
				}
			}
		}
	}

	launchOne.type = FBResource::Inst().NextBubble();
	for(int i = 0;i< 100 && dataNum[launchOne.type] <= 0;i++)
	{
		launchOne.type = FBResource::Inst().NextBubble();
	}

	nextOne.type = 	FBResource::Inst().NextBubble();
	for(int i = 0;i< 100 && dataNum[nextOne.type] <= 0;i++)
	{
		nextOne.type = 	FBResource::Inst().NextBubble();
	}
}

//key position
//left = 190, top = 44, between col = 32, between row = 28
//row%2 == 1, shift left 16, so this row hasn't 1st position ball

FBBubble::FBBubble(BitmapArray* p, BitmapArray* pFixed,BitmapArray* pFrozen, Bitmap* pBlink)
	: compressorPos(0),myId(0)
{
	endOfGame = 0;
	bitmap = p;
	fixedAnimation = pFixed;
	frozen = pFrozen;
	blinkBitmap = pBlink;
	blinkNum = 0;
	//int currentLevel[8*13] = 
	//{
	//	6,6,4,4,2,2,3,3,
	//	6,6,4,4,2,2,3,0,
	//	2,2,3,3,6,6,4,4,
	//	2,3,3,6,6,4,4,0,
	//};

	//for (int j=0 ; j<12 ; j++) 
	//{
	//	for (int i=j%2 ; i<8 ; i++) 
	//	{
	//		if (currentLevel[i+j*8]) 
	//		{
	//			(myData[i][j]).type = currentLevel[i+j*8];
	//			(myData[i][j]).l = 190+i*32-(j%2)*16;
	//			(myData[i][j]).t = 44+j*28;
	//			(myData[i][j]).h = 32;
	//			(myData[i][j]).w = 32;
	//		}
	//	}
	//}

	SetLevel(1);

	nextOne.type = 1;
	nextOne.l = 302;
	nextOne.t = 440;
	nextOne.h = 32;
	nextOne.w = 32;

	launchOne.type = 2;
	launchOne.l = 302;
	launchOne.t = 390;
	launchOne.h = 32;
	launchOne.w = 32;
}

void FBBubble::Render(int x, int y,double zoom)
{
	Bitmap *p = NULL;
	for (int j=0 ; j<12 ; j++) 
	for (int i=0 ; i<8 ; i++) 
	{
		if((myData[i][j]).type)
		{
			if((p = bitmap->Get((myData[i][j]).type-1)))
			{
				p->Draw(myData[i][j].l+x,myData[i][j].t+y,zoom);
			}
		}

		if((myData[i][j]).fixed)
		{

			if((p = fixedAnimation->Get((myData[i][j]).fixed-1)))
			{
				p->Draw(myData[i][j].l+x,myData[i][j].t+y,zoom);
			}
			(myData[i][j]).fixed--;
		}
	}

	if((p = bitmap->Get(nextOne.type-1)))
	{
		p->Draw(nextOne.l+x,nextOne.t+y,zoom);
	}
	if((p = bitmap->Get(launchOne.type-1)))
	{
		p->Draw(launchOne.l+x,launchOne.t+y,zoom);
	}

	for (std::vector<Data>::iterator i = movingOnes.begin();
		i != movingOnes.end();i++) 
	{
		if(i->state == Data::DEFAULT)
			continue;

		if(i->type)
		{
			if((p = bitmap->Get(i->type-1)))
			{
				p->Draw(i->l+x,i->t+y,zoom);
			}
		}
	}

	if(endOfGame)
	{
		if(endOfGame <= 12*8)
		{
			endOfGame += 4;
		}
		else
		{ 
			if((p = frozen->Get(launchOne.type-1)))
			{
				p->Draw(launchOne.l+x,launchOne.t+y,zoom);
			}
			if((p = frozen->Get(nextOne.type-1)))
			{
				p->Draw(nextOne.l+x,nextOne.t+y,zoom);
			}
		}

		for (int j=0 ; j<12 ; j++) 
		{
			for (int i=0 ; i<8 ; i++) 
			{
				if((myData[i][j]).type)
				{
					if((p = frozen->Get((myData[i][j]).type-1)))
					{
						p->Draw(myData[i][j].l+x,myData[i][j].t+y,zoom);
					}
				}

				if(j*8 + i > endOfGame)
					break;
			}
		}
	}
	else if(blinkNum)
	{

		int column = blinkNum/2;

		for (int i=0 ; i<13 ; i++) 
		{
			if((myData[column][i]).type)
			{
				blinkBitmap->Draw(myData[column][i].l+x,myData[column][i].t+y,zoom);

			}
		}

		blinkNum = 0;
	}
}

bool FBBubble::MovingBubbleExist()
{
	for (std::vector<Data>::iterator i = movingOnes.begin();
		i != movingOnes.end();i++) 
	{
		if(i->state != Data::DEFAULT)
			return true;
	}
	return false;
}

void FBBubble::MoveDown()
{
	for (int j=0 ; j<12 ; j++) 
	{
		for (int i=0; i<8 ; i++) 
		{
			if ((myData[i][j]).type) 
			{
				(myData[i][j]).t += 28;
			}
		}
	}

	compressorPos++;
	return;
}

bool FBBubble::CheckFail()
{
	if(compressorPos >= 12)
		return true;

	for (int i=0 ; i<8 ; i++) 
	{
		if((myData[i][12-compressorPos]).type)
		{
			return true;
		}
	}

	return false;
}

bool FBBubble::CheckSuccess()
{
	for (int j=0 ; j<12 ; j++) 
	for (int i=0 ; i<8 ; i++) 
	{
		if((myData[i][j]).type)
		{
			return false;
		}
	}

	return true;
}

//return row,col from 0
FBBubble::Pos FBBubble::GetCurrentPos(Data& data)
{
	int cx = data.l + data.w/2;
	int cy = data.t + data.h/2;

	Pos pos;

	pos.row = (cy - 44 - 28*compressorPos)/28;

	pos.col = (cx + (pos.row%2)*16 - 190)/32;

	return pos;
}

void FBBubble::GetCurrentLeftTop(int row,int col, int &left, int &top)
{
	left = 190+col*32-(row%2)*16;
	top = 44+(row+compressorPos)*28;
	return;
}

//maximum 8, initial -1
void FBBubble::GetNeighbors(Pos cur, Pos pos[])
{
	int i = 0;

	int row = cur.row;
	int col = cur.col;

	if ((row % 2) == 0) 
	{
		pos[i].row = row;
		pos[i].col = col + 1;
		i++;;
		pos[i].row = row;
		pos[i].col = col - 1;
		i++;
		pos[i].row = row+1;
		pos[i].col = col;
		i++;
		pos[i].row = row+1;
		pos[i].col = col+1;
		i++;
		pos[i].row = row-1;
		pos[i].col = col+1;
		i++;
		pos[i].row = row-1;
		pos[i].col = col;
		i++;
	}
	else
	{
		pos[i].row = row;
		pos[i].col = col + 1;
		i++;;
		pos[i].row = row;
		pos[i].col = col - 1;
		i++;
		pos[i].row = row+1;
		pos[i].col = col - 1;
		i++;
		pos[i].row = row+1;
		pos[i].col = col;
		i++;
		pos[i].row = row-1;
		pos[i].col = col - 1;
		i++;
		pos[i].row = row-1;
		pos[i].col = col;
		i++;
	}
    
	return;
}

//x - left, y - top
//891?????
bool FBBubble::CheckCollision(int x,int y,Pos pos[])
{
	for(int i = 0;i<8;i++)
	{
		if(pos[i].row < 0 
			|| pos[i].row > 12
			|| pos[i].col < 0
			|| pos[i].col > 7)
			continue;

		if(!myData[pos[i].col][pos[i].row].type)
			continue;

		if((myData[pos[i].col][pos[i].row].l - x)*(myData[pos[i].col][pos[i].row].l - x)
			+(myData[pos[i].col][pos[i].row].t - y)*(myData[pos[i].col][pos[i].row].t - y) < 891)
			return true;
	}

	return false;
}


//Pos cur has already updated according to myData
void FBBubble::GetJumps(int id,Pos cur,std::vector<Pos>& posList,Pos neighbors[])
{
	myData[cur.col][cur.row].id = id;

	posList.push_back(cur);

	for(int i = 0;i<8;i++)
	{
		if(neighbors[i].row < 0 
			|| neighbors[i].row > 12
			|| neighbors[i].col < 0
			|| neighbors[i].col > 7)
			continue;

		volatile int row = neighbors[i].row;
		volatile int col = neighbors[i].col;

		if(!myData[col][row].type)
			continue;

		if(myData[col][row].type == myData[cur.col][cur.row].type)
		{
			if(myData[col][row].id == id)
				continue;
			Pos neighbors2[8];
			GetNeighbors(neighbors[i],neighbors2);
			GetJumps(id,neighbors[i],posList,neighbors2);
		}
	}
}


//Pos cur has already updated according to myData
void FBBubble::CheckFalls(int id,Pos cur)
{
	if(myData[cur.col][cur.row].id == id)
		return;

	myData[cur.col][cur.row].id = id;

	Pos neighbors[8];

	GetNeighbors(cur, neighbors);

	for(int i = 0;i<8;i++)
	{
		if(neighbors[i].row < 0 
			|| neighbors[i].row > 12
			|| neighbors[i].col < 0
			|| neighbors[i].col > 7)
			continue;

		int row = neighbors[i].row;
		int col = neighbors[i].col;

		if(myData[col][row].type)
		{
			CheckFalls(id,neighbors[i]);
		}
	}
}

//x, y screen coordinate
void FBBubble::Fire(int x, int y, float& angle)
{

	if(y >= (390+32+8))
		return;

	if(y >= (390+32) && x > (302+32))
	{
		angle = 85;
		return Fire(ANGLE(angle));
	}
	else if(y >= (390+32) && x < (302+32))
	{
		angle = -85;
		return Fire(ANGLE(angle));
	}

	angle = atan((float)(x - 302-16)/(float)(y - 390-16));

	return Fire(angle);
}

void FBBubble::CalculateAngle(int x, int y, float& angle)
{
        
	angle = atan((float)(x - 302-16)/(float)(y - 390-16));
    
	if(angle >= ANGLE(85))
    {
        angle = ANGLE(85);
    }
    else if(angle <= ANGLE(-85))
    {
        angle = ANGLE(-85);
    }
    
    return;
}


void FBBubble::Fire(float angle)
{

	if(angle >= ANGLE(85) || angle <= ANGLE(-85))
		return;

	Data data = launchOne;
	data.state = Data::MOVEING;
	data.angle = angle;
	data.tick = 0;

	for(std::vector<Data>::iterator i = movingOnes.begin();
		i != movingOnes.end();i++)
	{
		if(i->state == Data::DEFAULT)
		{
			*i = data;
			goto FUNC_EXIT;
		}
	}

	movingOnes.push_back(data);

FUNC_EXIT:
    
    if(GlobalSetting::Inst().SoundOn)
        Sample::Inst().Play(LAUNCH);
    
	launchOne.type = nextOne.type;
	nextOne.type = 	FBResource::Inst().NextBubble();
	for(int i = 0;i< 100 && dataNum[nextOne.type] <= 0;i++)
	{
		nextOne.type = 	FBResource::Inst().NextBubble();
	}
}

int FBBubble::Update()
{
	int result = 0;

	std::vector<Data> jumpList;

	for(std::vector<Data>::iterator i = movingOnes.begin();
		i != movingOnes.end();i++)
	{
		if(i->state == Data::MOVEING)	//moving
		{
			int flag = 0;

			i->t = 390 + (double)i->tick* SPEED_MOVING * -cos(i->angle);	//MAX_BUBBLE_SPEED = 8,  FALL_SPEED = 1
			i->l = 302 + (double)i->tick* SPEED_MOVING * -sin(i->angle);

			if(i->angle >= 0 && i->l <= 190)
			{
				int num    = (190 - i->l) / (414-190+1);
				int remain = (190 - i->l) % (414-190+1);

				if(num % 2 == 0)
				{
					i->l = 190 + remain;
					flag = 1;
				}
				else
				{
					flag = 2;
					i->l = 414 - remain;
				}
			}
			else if(i->angle <= 0 && i->l >= 414)
			{
				int num    = (i->l - 414) / (414-190+1);
				int remain = (i->l - 414) % (414-190+1);

				if(num % 2 == 0)
				{
					flag = 1;
					i->l = 414 - remain;
				}
				else
				{
					flag = 2;
					i->l = 190 + remain;
				}
			}

			i->tick++;
			i->t = 390 + (double)i->tick* SPEED_MOVING * -cos(i->angle);	//MAX_BUBBLE_SPEED = 8,  FALL_SPEED = 1
			i->l = 302 + (double)i->tick* SPEED_MOVING * -sin(i->angle);

			if(i->angle >= 0 && i->l <= 190)
			{
				int num    = (190 - i->l) / (414-190+1);
				int remain = (190 - i->l) % (414-190+1);

				if(num % 2 == 0)
				{
					if(flag == 0 || flag == 2)
                    {
                        if(GlobalSetting::Inst().SoundOn)
                            Sample::Inst().Play(REBOUND);
                    }
					i->l = 190 + remain;
				}
				else
				{
					if(flag == 0 || flag == 1)
                    {
                        if(GlobalSetting::Inst().SoundOn)
                            Sample::Inst().Play(REBOUND);
                    }
					i->l = 414 - remain;
				}
			}
			else if(i->angle <= 0 && i->l >= 414)
			{
				int num    = (i->l - 414) / (414-190+1);
				int remain = (i->l - 414) % (414-190+1);

				if(num % 2 == 0)
				{
					if(flag == 0 || flag == 2)
                    {
                        if(GlobalSetting::Inst().SoundOn)
                            Sample::Inst().Play(REBOUND);
                    }
					i->l = 414 - remain;
				}
				else
				{
					if(flag == 0 || flag == 1)
                    {
                        if(GlobalSetting::Inst().SoundOn)
                            Sample::Inst().Play(REBOUND);
                    }
					i->l = 190 + remain;
				}
			}

			//check collision
			Pos pos = GetCurrentPos(*i);
			Pos neighbors[8];
			GetNeighbors(pos, neighbors);
			if(CheckCollision(i->l,i->t,neighbors) || i->t < 44 + compressorPos*28)
			{
//				//remove it, hyjiang
//				if(myData[pos.col][pos.row].type)
//				{
//					pos = lastPos;
//					GetNeighbors(pos, neighbors);
//				}

				i->state = Data::DEFAULT;

				if(i->t + 14 > 44+12*28)	//check fail
				{
					i->t -= 7;
					myData[pos.col][pos.row] = *i;
					endOfGame = 1;
					return UPDATE_FAILED;
				}

				GetCurrentLeftTop(pos.row,pos.col,i->l,i->t);
				myData[pos.col][pos.row] = *i;
				dataNum[i->type]++;

				//test purpose, remove it, hyjiang
//				Pos pos__ = GetCurrentPos(*i);
//				if(pos__.row != pos.row || pos__.col != pos.col)
//				{
//					GetNeighbors(pos__, neighbors);					
//				}

				//check jump
				std::vector<Pos> posList;
				GetJumps(++myId,pos,posList,neighbors);
				if(posList.size() >= 3)
				{
					//jumping
					for(std::vector<Pos>::iterator j = posList.begin();
						j != posList.end();j++)
					{
						Data data = myData[j->col][j->row];
						myData[j->col][j->row].type = 0;

						data.state = Data::JUMPING;
						data.jumpingx = FBResource::Inst().JumpingX();
						data.jumpingy = FBResource::Inst().JumpingY();
						data.fallingy = FBResource::Inst().FallingY();

						jumpList.push_back(data);
					}

					//check fall
					int id = ++myId;
					for (int i=0 ; i<8 ; i++) 
					{
						if (myData[i][0].type) 
						{
							CheckFalls(id,GetCurrentPos(myData[i][0]));
						}
					}

					for (int i=0 ; i<12 ; i++) 
					{
						for (int j=0 ; j<8 ; j++) 
						{
							if(myData[j][i].type && myData[j][i].id != id)
							{
								Data data = myData[j][i];
								myData[j][i].type = 0;
								data.state = Data::JUMPING;
								data.jumpingx = FBResource::Inst().JumpingX();
								data.jumpingy = FBResource::Inst().JumpingY();
								data.fallingy = FBResource::Inst().FallingY();

								jumpList.push_back(data);
							}
						}
					}

					//play sound
                    if(GlobalSetting::Inst().SoundOn)
                        Sample::Inst().Play(DESTROY_GROUP);

					result = UPDATE_FALLING;

				}
				else
				{
					(myData[pos.col][pos.row]).fixed = 6;
                    if(GlobalSetting::Inst().SoundOn)
                        Sample::Inst().Play(STICK);
					result = UPDATE_ONEBUBBLE_FIXED;
				}

			}

//			//remove it, hyjiang
//			if(pos.col != lastPos.col || pos.row != lastPos.row)
//			{
//				lastPos = pos;
//			}
		}
		if(i->state == Data::JUMPING)	//jumping
		{
			i->jumpingy += SPEED_JUMPING;
			i->l += i->jumpingx;
			i->t += i->jumpingy;

			if(i->t >= 680)
			{
				i->state = Data::FALLING;
			}
		}

		if(i->state == Data::JUMPING || i->state == Data::FALLING)	//falling
		{
			i->jumpingy += SPEED_FALLING;
			i->t += i->jumpingy;

			if(i->t >= 680)
			{
				i->state = Data::DEFAULT;
			}
		}
	}

	for(std::vector<Data>::iterator j = jumpList.begin();
		j != jumpList.end();j++)
	{
		dataNum[j->type]--;
		std::vector<Data>::iterator i = movingOnes.begin();
		for(;i != movingOnes.end();i++)
		{
			if(i->state == Data::DEFAULT)
			{
				*i = *j;
				break;
			}
		}

		if(i == movingOnes.end())
			movingOnes.push_back(*j);
	}

	return result;
}
