#ifndef VOBJ_H_INCLUDED
#define VOBJ_H_INCLUDED

#include "vNode.h"
//using namespace std;

/**voxel cube is divided into 8
 * bone is a start point and angles of rotation, all points on the object are rotated around it
 * each voxel is assumed to the same shape as its parent, with each dimension being half the size, to allow the bounding box to be easily determined
 * static objects dont have bones, but act like a single bone
 */

 //TODO generalize the intersection formula to work with more transforms

struct pixel{
	/**needs:
	* rgb as double if post processing effects and uint8 if not
	* distance as double
	* alpha as 0-255
	*/
	double r,g,b,d;
	uint8_t a;
};

struct coords{
	double x,y,z;
	coords(double x0=0,double y0=0,double z0=0){
		x=x0;
		y=y0;
		z=z0;
	}
};

class corners{
	coords edge[8];
	public:
	coords operator[](int i){return edge[i];}
	~corners(){delete[] edge;}
};

struct vector3d{
	public:
	double x,y,z,dx,dy,dz;
	vector3d(double x=0,double y=0,double z=0,double dx=0,double dy=0,double dz=0){
		this->x=x;
		this->y=y;
		this->z=z;
		this->dx=dx;
		this->dy=dy;
		this->dz=dz;
	}
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

	/**i dont like this, redoing
	#define sphere true
	///vec is a vector
	///scale is the scale of the current voxel
	bool intersects(vector3d vec,coords scale){
		const double d=vec.dx,e=vec.dy,f=vec.dz;
		const double
		x0=vec.x-this->x,
		y0=vec.y-this->y,
		z0=vec.z-this->z;

		#if sphere
		const double
		rx=this->sx*this->sx,
		ry=this->sy*this->sy,
		rz=this->sz*this->sz;

		const double
		a=d*d/rx+e*e/ry+f*f/rz,
		b=d*x0/rx+e*y0/ry+f*z0/rz,
		c=x0*x0/rx+y0*y0/ry+z0*z0/rz;
		return !(b*b<a*c);
		#else
		const double
		a=scale.x/2,
		b=scale.y/2,
		c=scale.z/2;

		//vector<double> vl,vh;
		double max,mina,tmp;
		if(d==0 && abs(x0)>a){return false;}
		else{
			//vl.push_back(-(a+x0)/d);
			//vh.push_back((a-x0)/d);
			min=-(a+x0)/d;
			max=(a-x0)/d;
		}

		if(e==0 && abs(y0)>b){return false;}
		else{
			//vl.push_back(-(b+y0)/e);
			//vh.push_back((b-y0)/e);
			tmp=-(b+y0)/e;
			min=(tmp<min)?tmp:min;
			tmp=(b-y0)/e;
			max=(tmp>max)?tmp:max;
		}

		if(f==0 && abs(z0)>c){return false;}
		else{
			//vl.push_back(-(c+z0)/f);
			//vh.push_back((c-z0)/f);
			tmp=-(b+y0)/e;
			min=(tmp<min)?tmp:min;
			tmp=(b-y0)/e;
			max=(tmp>max)?tmp:max;
		}

		return min<=max;

		#endif
	}//*/

	//does the object intersect the vector
	bool intersects(vector3d vec){

	}
	///does a vector intersect the voxel node, the scale is what to multiply the object's width by
	bool intersects(vNode vox,vector3d* vec,coords* center,double scale){//smallest possable values are passed, that is the explaination for pointers here
		const double//shifts the vector's start so that the object is centered
		x0=vec->x-this->x,
		y0=vec->y-this->y,
		z0=vec->z-this->z;

		const double
		a=(xvec.dx+yvec.dx+zvec.dx)*scale,
		b=(xvec.dy+yvec.dy+zvec.dy)*scale,
		c=(xvec.dz+yvec.dz+zvec.dz)*scale;

		const double
		d=vec->dx,
		e=vec->dy,
		f=vec->dz;

		double max=-INFINITY,min=INFINITY,tmp;//note to self: max float is FLT_MAX, max double is DBL_MAX, and max long double is LDBL_MAX
		if(d==0 && abs(x0)>a){return false;}
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

	//return the color of the voxel which the vector intersects
	pixel draw(vector3d norm){
	}
};

struct vRef{
	vObj* point;
	vRef(vObj* to){
		point=to;
	}
	vRef(){vRef(NULL);}
};

#endif // VOBJ_H_INCLUDED
