#ifndef VBONE_H_INCLUDED
#define VBONE_H_INCLUDED

#include "vObj.h"

class vBone:public vObj{
	public:
	bool deform;//if true, voxel group stretches, false then just moves, think mittens vs fingers
	coords start,point;//all transforms are based on the angle formed by start, the point from its parent, and this point

};

#endif // VBONE_H_INCLUDED
