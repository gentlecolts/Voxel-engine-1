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
	vector3d xmag,ymag,xcomp,ycomp;//xmag and ymag are from the origin and have the width and height only, xcomp and ycomp are
	double width,height;///consider removing these of these and replacing them with the ratio of height to width, and assume width is 1, only the ratio matters with the vector projection method
	double viewAng;//angle formed by the camera origin the widthwise bounds of the surface
	double zoom;//distance from origin to screen center
	double x,y,z;

	public:
	int px,py;//the actual # of pixels on the screen

	vCamera(int xpixels=0,int ypixels=0,double xstart=0,double ystart=0,double zstart=0,double incstart=0,double azstart=0,double viewangle=90,double defZoom=1){
		px=xpixels;
		py=ypixels;
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

	void pnt2scrn(double x,double y,double z,double* kx,double* ky){
		/**find the vector from the camera origin to the point
		 * intersect that vector with the plane that the screen is on
		 * get the vector from the screen center to the point
		 * split it into the components of xcomp and ycomp
		 * the constants of this split tell where it is reletive to the center of the actual screen
		 * (pixWidth*(kw+1)/2,pixHeight-pixHeight*(kh+1)/2)
		 * aka
		 * (pixWidth*(kw+1)/2,pixHeight*(1-kh)/2)
		 */
		vector3d vec,plane;
		vec.x=this->x;
		vec.y=this->y;
		vec.z=this->z;
		vec.dx=x-this->x;
		vec.dy=y-this->y;
		vec.dz=z-this->z;

		plane=getNormal();

		double t,a,b,c,d;
		a=plane.dx;
		b=plane.dy;
		c=plane.dz;
		d=a*(plane.x)+b*(plane.y)+c*(plane.z);
		t=(d-a*(this->x)-b*(this->y)-c*(this->z))/(a*(vec.dx)+b*(vec.dy)+c*(vec.dz));

		a=t*(this->x+t*(x-this->x)-plane.x);//recycling a,b, and c
		b=t*(this->y+t*(y-this->y)-plane.y);
		c=t*(this->z+t*(z-this->z)-plane.z);
		if((xcomp.dx)*(ycomp.dy)!=(xcomp.dy)*(ycomp.dx)){
			*kx=(a*(ycomp.dy)-b*(ycomp.dx))/((xcomp.dx)*(ycomp.dy)-(xcomp.dy)*(ycomp.dx));
		}else if((xcomp.dx)*(ycomp.dz)!=(xcomp.dz)*(ycomp.dx)){
			*kx=(a*(ycomp.dz)-c*(ycomp.dx))/((xcomp.dx)*(ycomp.dz)-(xcomp.dz)*(ycomp.dx));
		}else{
			*kx=(c*(ycomp.dy)-b*(ycomp.dz))/((xcomp.dz)*(ycomp.dy)-(xcomp.dy)*(ycomp.dz));
		}

		if(ycomp.dx!=0){*ky=(a-*kx*(xcomp.dx))/ycomp.dx;}
		else if(ycomp.dy!=0){*ky=(b-*kx*(xcomp.dy))/ycomp.dy;}
		else{*ky=(c-*kx*(xcomp.dz))/ycomp.dz;}
	}/*
	void proj2d(double x,double y,double z,int *x2,int *y2){//x,y,z are the point to project down
		double kx,ky;
		pnt2scrn(x,y,z,&kx,&ky);

	}//*/
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
		#define method 1
		#if method==0
		//assume the voxel is facing you, is a cube, and the max length is the len for all sides
		//should be faster at the cost of false positives near the edges, they wont get
		double maxs=max(obj->sx,max(obj->sy,obj->sz));
		#elif method==1
		//intersect all vectors of the camera origin and the voxel corners with the screen, if any intersect, then return true
		//reasoning being that the intersection formula might be faster then the projection one
		corners corn=obj->getCorners();
		#endif
	}

	vector3d getNormal(){
		vector3d vec;
		vec.dx=zoom*sinc*caz;
		vec.dy=zoom*sinc*saz;
		vec.dz=zoom*cinc;
		vec.x=x+vec.dx;
		vec.y=y+vec.dy;
		vec.z=z+vec.dz;
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

		for(int i=0;i<objarr->size();i++){
			if(canSee(&(*objarr)[i])){
				draw(&(*objarr)[i],&norm,parr);
			}
		}
	}

	///changing the width or height require that the vectors are updated, so does rotation
	double getInc(){return inc;}
	double getAz(){return az;}
	void setInc(double amount){inc=amount;sinc=sin(inc);cinc=cos(inc);}
	void setAz(double amount){az=amount;saz=sin(az);caz=cos(az);}
	void addInc(double amount){setInc(inc+amount);}
	void addAz(double amount){setAz(az+amount);}
	double getViewAngle(){return viewAng;}
	void setViewAngle(double newAng){viewAng=newAng;}///also needs to set width
	double getWidth(){return width;}///also needs to set viewangle
	double getHeight(){return height;}
	void setWidth(double w){}
	void setHeight(double h){height=h;}

	///note to self, get coords, centers of component vecs should still have old dist, adjust origin or screen center to the new dist
	///potentiall helpfull, points have same inclination and azimuth, but different radius
	///also changes viewagle
	double getZoom(){return zoom;}
	void setZoom(double newZoom){zoom=newZoom;}
};

#endif // VCAMERA_H_INCLUDED
