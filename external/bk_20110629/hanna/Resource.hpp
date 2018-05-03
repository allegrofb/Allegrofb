#ifndef RESOURCE_H
#define RESOURCE_H

class Resource
{
public:
	Resource(Drawing* p = new Drawing()){ draw = p;}
	virtual ~Resource(){if(draw)delete draw;};
	Drawing* draw;		
};

#endif	//RESOURCE_H

