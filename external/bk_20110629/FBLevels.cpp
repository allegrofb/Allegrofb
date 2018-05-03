#include "FBLevels.hpp"
#include "hanna/Alo.hpp"

FBLevels* FBLevels::LoadFrom(const char* filename)
{
	ALLEGRO_FILE* file = al_fopen(util_get_path(filename),"r");
	if(file == NULL)
	{
		return NULL;
	}
	FBLevels* p = new FBLevels();
	
	char buf[80] = {0};
	int level[8*13] = {0};
	int row = 0;
	int col = 0;
	while(al_fgets(file,buf,80))
	{
		if(buf[0] == 0x0A)
		{
			p->Add(level);
			row = 0;
			continue;
		}
		
		if(row > 10)
			continue;

		for(int i = 0,col = row % 2?1:0;buf[i] && col <=8;i++)
		{
			if(buf[i] >= '0' && buf[i] <= '9' )
			{
				level[row*8+col] = buf[i] - '0' + 1;
				col++;
			}
			else if(buf[i] == '-')
			{
				level[row*8+col] = 0;
				col++;
			}
		}
		
		row++;
	}
	
	al_fclose(file);
	return p;
}

FBLevels::FBLevels()
{
	
}

FBLevels::~FBLevels()
{
	for(std::vector<int*>::const_iterator i = levels.begin(),m = levels.end();i!=m;i++)
	{
		delete[] *i;
	}
}

void FBLevels::Add(int* buf)
{
	int* level = new int[8*13];
	memcpy(level,buf,sizeof(int)*8*13);
	levels.push_back(level);
}

//i from 1 ... 
const int* FBLevels::GetLevel(int i)
{
	if(i > (int)levels.size() || i <= 0)
	{
		return levels[0];
	}
	return levels[i-1];
}

int FBLevels::GetMax()
{	
	return levels.size();
}