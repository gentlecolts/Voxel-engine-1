#ifndef VCAMERA_H_INCLUDED
#define VCAMERA_H_INCLUDED

#include "vStatic.h"

float invsqrt(float x){
	float xhalf=0.5f*x;
	int i=*(int*)&x;
	i=0x5f3759df-(i>>1);
	x=*(float*)&i;
	x=x*(1.5f-xhalf*x*x);
	return x;
}

#define sqrt(x) (1.0/invsqrt(x))

struct plane{
	double a,b,c,d;
};

//TODO: add tilt
class vCamera{
	private:
	double inc,az,sinc,saz,cinc,caz;//inclination and aximuth, and their sine and cosine
	plane scrn;

	public:
	double width,height;
	double x,y,z;
	double viewAng;//angle formed by the camera origin the widthwise bounds of the surface
	double zoom;
	vector3d dir;//use whenever a vector is needed

	vCamera(double xstart=0,double ystart=0,double zstart=0,double incstart=0,double azstart=0,double viewangle=90,double defZoom=1){
		x=xstart;
		y=ystart;
		z=zstart;
		viewAng=viewangle;
		zoom=defZoom;

		inc=incstart;
		az=azstart;
		sinc=sin(inc);
		saz=sin(az);
		cinc=cos(inc);
		caz=cos(az);
	}

	operator coords(){
		coords cam;
		cam.x=x;
		cam.y=y;
		cam.z=z;
		return cam;
	}

	//takes a point and rotates it in the opposite direction that the camera is rotated
	//usefull for alligning things to the axis
	void unrot(double x,double y,double z,double* x2,double* y2,double*z2){
		double dx,dy,dz,r,sr1,sr2,sr3;//consider making these private class vars if the program is too slow
		dx=x-this->x;//position if camera is center
		dy=y-this->y;
		dz=z-this->z;
		r=dx*dx+dy*dy+dz*dz;

		///see notes.txt for formula derivations

		sr1=sqrt((r*r)/(z*z)-1);
		sr2=sqrt((y*y)/(x*x)+1);
		*x2=z*(sr1*cinc-sinc)*(caz+y/x*saz)/sr2;
		*y2=z*(sr1*cinc-sinc)*((y/x)*caz-saz)/sr2;
		*z2=z*(cinc+sr1*sinc);
	}
	void proj2d(double x,double y,double z,double *x2,double *y2){//x,y,z are the point to project down
		double x3,y3,z3;

		unrot(x,y,z,&x3,&y3,&z3);

		*x2=x3*zoom/(zoom+z3);
		*y2=y3*zoom/(zoom+z3);
	}
	bool canSee(vObj* obj){
		/**for each vertex of the cube
		 *  find normal to camera origin
		 *  find normal to object center
		 * if all solving t's have the same sign return false
		 * else:
		 *  can any point intersect the infinite square pyramid (pyramid of vision) created by the camera?
		 *  if yes return true
		 * return false
		 */
		#define method 2
		#if method==0
		//intersect cube with
		corners corn=obj->getCorners();
		coords c;
		double x2,y2,z2;
		for(int i=0;i<8;i++){
			c=corn[i];
			c.x-=this->x;
			c.y-=this->y;
			c.z-=this->z;

			unrot(c.x,c.y,c.z,&x2,&y2,&z2);//remember to make sure that this alligns it properly

			//after rotating, z should be positive if the camera is pointed towards the point, meaning all negative z will make this return false
			if(abs(x2)>z2*width/2 || abs(y2)>z2*height/2){return true;}
		}
		return false;
		#elif method==1
		//assume the voxel is facing you, is a cube, and the max length is the len for all sides
		//should be faster at the cost of false positives near the edges, they wont get
		double maxs=max(obj->sx,max(obj->sy,obj->sz));
		#elif method==2
		//intersect all vectors of the camera origin and the voxel corners with the screen, if any intersect, then return true
		//reasoning being that the intersection formula might be faster then the projection one
		corners corn=obj->getCorners();
		#endif
	}

	vector3d getNormal(){
		vector3d vec;
		vec.x=x;
		vec.y=y;
		vec.z=z;
		vec.dx=zoom*sinc*caz-x;
		vec.dy=zoom*sinc*saz-y;
		vec.dz=zoom*cinc-z;
		return vec;
	}

	//use each vector from this to call obj's draw and render it to parr
	//first find the 2d bounding box of the object:find the min and max coords of the
	//
	void draw(vObj* obj,vector3d* norm,pixel parr[]){
	}

	//PRECONDITION: parr is size px*py
	void rendView(int px,int py,vector<vObj>* objarr,vStatic *scene,pixel parr[]){
		/**for each object in objarr:
		 * if bounding box is in the camera's view
		 * project bounding box coords to find min and max x and y
		 * raytrace that object from those bounds
		 */
		//note a rectangular prism, centerd at (0,0,0), can be defined as |x|<=a/2 && |y|<=b/2 && |z|<=c/2 where a,b,and c are the side lengths
		/** given a,b,c,d,e,f,x0,y0,z0
		|x|<=a/2
		|y|<=b/2
		|z|<=c/2
		dt+x0=x
		et+y0=y
		ft+z0=z
		*/

		vector3d norm=getNormal();
		//width=
		coords center,topleft,bottomright;
		center.x=zoom*sinc*caz;
		center.y=zoom*sinc*saz;
		center.z=zoom*cinc;
		scrn.a=norm.dx;
		scrn.b=norm.dy;
		scrn.c=norm.dz;
		scrn.d=-(norm.dx*center.x+norm.dy*center.y+norm.dz*center.z);

		scene->draw(norm);
		height=py*width/px;

		for(int i=0;i<objarr->size();i++){
			if(canSee(&(*objarr)[i])){
				draw(&(*objarr)[i],&norm,parr);
			}
		}
	}

	double getInc(){return inc;}
	double getAz(){return az;}
	void setInc(double amount){inc=amount;sinc=sin(inc);cinc=cos(inc);}
	void setAz(double amount){az=amount;saz=sin(az);caz=cos(az);}
	void addInc(double amount){setInc(inc+amount);}
	void addAz(double amount){setAz(az+amount);}
};

#endif // VCAMERA_H_INCLUDED
