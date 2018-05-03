#ifndef FBLEVELS_H
#define FBLEVELS_H

#include <vector>

class FBLevels
{
public:
	static FBLevels* LoadFrom(const char* filename);
	~FBLevels();		
	const int* GetLevel(int i);
	int GetMax();
private:
	void Add(int*);
	FBLevels();
	std::vector<int*> levels;
};


#endif	//FBLEVELS_H

