#ifndef RESOURCEMGR_HPP
#define RESOURCEMGR_HPP

class ResourceManager {
public:
	virtual void Destroy(void) = 0;
	virtual bool Load(void) = 0;
};

class AsynLoader
{
public:
	AsynLoader(ResourceManager* p){myManager=p;myFlag = false;};

	void Start(){myManager->Load();myFlag = true;};
	bool IsFinished(){return myFlag;};

private:
	ResourceManager* myManager;
	bool myFlag;
};


#endif // RESOURCEMGR_HPP

