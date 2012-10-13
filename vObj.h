#ifndef VOBJ_H_INCLUDED
#define VOBJ_H_INCLUDED

#include "vNode.h"
//using namespace std;

/**voxel cube is divided into 8
 * bone is assumed to be a rectangle size 1.0,1.0,1.0 with x,y, and z scale variables
 * each voxel is assumed to the same shape as its parent, with each dimension being half the size, to allow the bounding box to be easily determined
 * static objects dont have bones, but act like a single bone
 */

struct coords{
	double x,y,z;
};

class corners{
	coords edge[8];
	public:
	coords operator[](int i){return edge[i];}
	~corners(){delete[] edge;}
};

class vObj{
	public:
	double x,y,z;
	double sx,sy,sz;

	static coords cam;

	vObj(double x_initial=0,double y_initial=0,double z_initial=0,double sx_initial=0,double sy_initial=0,double sz_initial=0){
		x=x_initial;
		y=y_initial;
		z=z_initial;
		sx=sx_initial;
		sy=sy_initial;
		sz=sz_initial;
	}

	operator double(){
		double dx=x-cam.x,dy=y-cam.y,dz=y-cam.z;
		return dx*dx+dy*dy+dz*dz;
	}

	corners getCorners(){

	}
};

#endif // VOBJ_H_INCLUDED
