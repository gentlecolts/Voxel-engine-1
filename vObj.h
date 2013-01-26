#ifndef VOBJ_H_INCLUDED
#define VOBJ_H_INCLUDED

#include "vNode.h"
//using namespace std;

/**voxel cube is divided into 8
 * each voxel is assumed to the same shape as its parent, with each dimension being half the size, to allow the bounding box to be easily determined
 */

 //TODO generalize the intersection formula to work with more transforms

 class corners{
	coords edge[8];
	public:
	coords operator[](int i){return edge[i];}
	~corners(){delete[] edge;}
};

class vObj{
	public:
	double x,y,z;
	//double sx,sy,sz;

	///essentially unit vectors for the cube, apply transformations to these to transform the entire object
	vector3d xvec,yvec,zvec;//note that the centers of these are ignored and x,y,and z are used instead

	vNode tree;
	/**TODO add a list of some form (linked would prob be best) of all vRefs that point to this
	 * there needs to be a way to make sure that if this object is deleted, pointers to it dont cause problems
	 * aka need to avoid dangling pointers
	 * any vRefs that refrence this object should have their pointer set to null when this object's deconstructor is called
	**/

	vObj(double x_initial=0,double y_initial=0,double z_initial=0,double sx_initial=1,double sy_initial=1,double sz_initial=1){
		/*
		x=x_initial;
		y=y_initial;
		z=z_initial;
		sx=sx_initial;
		sy=sy_initial;
		sz=sz_initial;//*/
		xvec.dx=sx_initial;
		xvec.dy=0;
		xvec.dz=0;
		yvec.dy=sy_initial;
		yvec.dx=0;
		yvec.dz=0;
		zvec.dz=sz_initial;
		zvec.dy=0;
		zvec.dx=0;
	}

	corners getCorners(){
	}

	//does the object intersect the vector
	bool intersects(vector3d vec){

	}
	//does a vector intersect the voxel node, the scale is what to multiply the object's width by
	bool intersects(vector3d* vec,coords* center,double scale){//size of pointers is smaller than size of vector3d or coords
		const double//shifts the vector's start so that the object is centered
		x0=(vec->x)-(center->x),//changed to center from this
		y0=(vec->y)-(center->y),
		z0=(vec->z)-(center->z);

		const double//sum of this object's component vectors scaled down to the size of the voxel node
		a=(xvec.dx+yvec.dx+zvec.dx)*scale,
		b=(xvec.dy+yvec.dy+zvec.dy)*scale,
		c=(xvec.dz+yvec.dz+zvec.dz)*scale;

		const double
		d=vec->dx,
		e=vec->dy,
		f=vec->dz;

		double max=-INFINITY,min=INFINITY,tmp;//note to self: max float is FLT_MAX, max double is DBL_MAX, and max long double is LDBL_MAX
		if(d==0 && abs(x0)>a){return false;}//if dx is 0 and the x-coordinate of the origin is outside of the box, then the vector does not intersect
		else{
			min=-(a+x0)/d;
			max=(a-x0)/d;
		}

		if(e==0 && abs(y0)>b){return false;}
		else{
			tmp=-(b+y0)/e;
			min=(tmp<min)?tmp:min;
			tmp=(b-y0)/e;
			max=(tmp>max)?tmp:max;
		}

		if(f==0 && abs(z0)>c){return false;}
		else{
			tmp=-(c+z0)/f;
			min=(tmp<min)?tmp:min;
			tmp=(c-z0)/f;
			max=(tmp>max)?tmp:max;
		}

		return min<=max;
	}

	///retraversing every time seems like a waste, make one that returns an array of pixel objects
	///(make a pixarr (watch for copyrights) class, that stores width and height and the array)
	//return the color of the voxel which the vector intersects
	//px and py are width and height of a pixel (assuemd equal), w,h are width and height of the voxel node (assumed equal, thats how its rendered)
	//px*py>w/(2^d)*h/(2^d)
	//2^(2*d)>w*w/(px*px)
	//2^d>w/px
	//d>log2(w/px)
	bool draw(vector3d vec, pixel* pix,){//note: the pointer is not an array
		int d,cur;
		coords center;
		center.x=x;
		center.y=y;
		center.z=z;

		const int maxdep=0;
	}

	//index is 0-7
	//0:1,1,1
	//1:1,1,-1
	//2:1,-1,1
	//3:1,-1,-1
	//4:-1,1,1
	//5:-1,1,-1
	//6:-1,-1,1
	//7:-1,-1,-1
	coords childCoord(int index){
		coords ret;
		ret.x=1;
		ret.y=1;
		ret.z=1;

		if(index<4){ret.x=-1;}
		if((index>>1)&1==1){ret.y=-1;}// (index/2) mod 2
		if(index&1==1){ret.z=-1;}
		return ret;
	}
};

///TODO
//preprogrammed refrence to a voxel, should handle memory management
struct vRef{
	vObj* point;
	vRef(vObj* to){
		point=to;
	}
	vRef(){vRef(NULL);}
};

#endif // VOBJ_H_INCLUDED
