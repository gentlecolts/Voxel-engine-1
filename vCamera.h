#ifndef VCAMERA_H_INCLUDED
#define VCAMERA_H_INCLUDED

#include <cmath>
#include "vStatic.h"
using namespace std;

float invsqrt(float x){
	float xhalf=0.5f*x;
	int i=*(int*)&x;
	i=0x5f3759df-(i>>1);
	x=*(float*)&i;
	x=x*(1.5f-xhalf*x*x);
	return x;
}

#define sqrt(x) (1.0/invsqrt(x))

struct pixel{
	/**needs:
	* rgb as double if post processing effects and uint8 if not
	* distance as double
	* alpha as 0-255
	*/
	double r,g,b,d;
	uint8_t a;
};

class vector3d{
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

struct plane{
	double a,b,c,d;
};

class vCamera{
	private:
	double x3,y3,z3,dx,dy,dz,r,sr1,sr2,sr3;//so that they dont need to be created and destroyed each time the projection is called
	double inc,az,sinc,saz,cinc,caz;//inclination and aximuth, and their sine and cosine
	double width;
	plane scrn;

	public:
	double x,y,z;
	double viewAng;
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

	void proj2d(double x,double y,double z,double *x2,double *y2){//x,y,z are the point to project down
		dx=x-this->x;//position if camera is center
		dy=y-this->y;
		dz=z-this->z;
		r=dx*dx+dy*dy+dz*dz;

		/**
		a=acos(z/r)-inc
		b=atan(y/x)-az
		r=sqrt(x*x+y*y+z*z)

		sin(acos(x))=sqrt(1-x^2)
		sin(atan(x))=x/sqrt(x*x+1)
		cos(atan(x))=1/sqrt(x*x+1)

		x=r*sin(a)*cos(b)
		x=r*sin(acos(z/r)-inc)*cos(atan(y/x)-az)
		x=r*(sin(acos(z/r))*cos(inc)-cos(acos(z/r))*sin(inc))*(cos(atan(y/x))*cos(az)+sin(atan(y/x))*sin(az))
		x=r*(sqrt(1-(z/r)^2)*cos(inc)-z/r*sin(inc))*(1/sqrt((y/x)^2+1)*cos(az)+(y/x)/sqrt((y/x)^2+1)*sin(az))
		x=(r*sqrt(1-(z/r)^2)*cos(inc)-z*sin(inc))*(cos(az)+y/x*sin(az))/sqrt((y/x)^2+1)
		x=(r*z/r*sqrt(1/(z/r)^2-1)*cos(inc)-z*sin(inc))*(cos(az)+y/x*sin(az))/sqrt((y/x)^2+1)
		x=z*(sqrt((r*r)/(z*z)-1)*cos(inc)-sin(inc))*(cos(az)+y/x*sin(az))/sqrt((y*y)/(x*x)+1)

		y=r*sin(a)*sin(b)
		y=r*sin(acos(z/r)-inc)*sin(atan(y/x)-az)
		y=r*(sin(acos(z/r))*cos(inc)-cos(acos(z/r))*sin(inc))*(sin(atan(y/x))*cos(az)-cos(atan(y/x))*sin(az))
		y=r*(sqrt(1-(z/r)^2)*cos(inc)-z/r*sin(inc))*((y/x)/sqrt((y/x)^2+1)*cos(az)-1/sqrt((y/x)^2+1)*sin(az))
		y=(r*sqrt(1-(z/r)^2)*cos(inc)-z*sin(inc))*((y/x)*cos(az)-sin(az))/sqrt((y/x)^2+1)
		y=(r*z/r*sqrt(1/(z/r)^2-1)*cos(inc)-z*sin(inc))*((y/x)*cos(az)-sin(az))/sqrt((y/x)^2+1)
		y=z*(sqrt((r/z)^2-1)*cos(inc)-sin(inc))*((y/x)*cos(az)-sin(az))/sqrt((y/x)^2+1)
		y=z*(sqrt((r*r)/(z*z)-1)*cos(inc)-sin(inc))*((y/x)*cos(az)-sin(az))/sqrt((y/x)^2+1)

		z=r*cos(a)
		z=r*cos(acos(z/r)-inc)
		z=r*(cos(acos(z/r))*cos(inc)+sin(acos(z/r))*sin(inc))
		z=r*(z/r*cos(inc)+sqrt(1-(z/r)^2)*sin(inc))
		z=(z*cos(inc)+r*sqrt(1-(z/r)^2)*sin(inc))
		z=(z*cos(inc)+r*z/r*sqrt(1/(z/r)^2-1)*sin(inc))
		z=(z*cos(inc)+z*sqrt((r*r)/(z*z)-1)*sin(inc))
		z=z*(cos(inc)+sqrt((r*r)/(z*z)-1)*sin(inc))
		z*=(cos(inc)+sqrt((r*r)/(z*z)-1)*sin(inc))
		*/

		sr1=sqrt((r*r)/(z*z)-1);
		sr2=sqrt((y*y)/(x*x)+1);
		x3=z*(sr1*cinc-sinc)*(caz+y/x*saz)/sr2;
		y3=z*(sr1*cinc-sinc)*((y/x)*caz-saz)/sr2;
		z3=z*(cinc+sr1*sinc);

		*x2=x3*zoom/(zoom+z3);
		*y2=y3*zoom/(zoom+z3);
	}

	bool canSee(vObj obj){
		/**for each vertex of the cube
		 *  find normal to camera origin
		 *  find normal to object center
		 * if all solving t's have the same sign return false
		 * else:
		 *  can any point intersect the infinite square pyramid created by the camera?
		 *  if yes return true
		 * return false
		 */
		corners corn=obj.getCorners();
	}

	#define sphere true
	/**vec is a vector formed by the camera and its origin
	 * cor is the senter of the current voxel
	 * scale is the scale of the current voxel*/
	bool intersects(vector3d vec,coords cor,coords scale){
		double a,b,c,d,e,f,x0,y0,z0;
		d=vec.dx;
		e=vec.dy;
		f=vec.dz;
		x0=vec.x-cor.x;
		y0=vec.y-cor.y;
		z0=vec.z-cor.z;
		#if sphere
		double rx,ry,rz;
		rx=scale.x*scale.x;
		ry=scale.y*scale.y;
		rz=scale.z*scale.z;

		a=d*d/rx+e*e/ry+f*f/rz;
		b=d*x0/rx+e*y0/ry+f*z0/rz;
		c=x0*x0/rx+y0*y0/ry+z0*z0/rz;
		return !(b*b<a*c);
		#else
		a=scale.x/2;
		b=scale.y/2;
		c=scale.z/2;

		vector<double> vl,vh;
		if(d==0 && abs(x0)>a){return false;}
		else{
			vl.push_back(-(a+x0)/d);
			vh.push_back((a-x0)/d);
		}

		if(e==0 && abs(y0)>b){return false;}
		else{
			vl.push_back(-(b+y0)/e);
			vh.push_back((b-y0)/e);
		}

		if(f==0 && abs(z0)>c){return false;}
		else{
			vl.push_back(-(c+z0)/f);
			vh.push_back((c-z0)/f);
		}

		return max(vl)<=min(vh);
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

	void draw(vObj *obj,vector3d norm){

	}

	//PRECONDITION: objarr is an array of visible voxel objects sorted by distance from this camera, parr is size px*py
	void rendView(int px,int py,vector<vObj*> objarr,vStatic *scene,pixel parr[]){
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

		draw(scene,norm);
		for(int i=0;i<objarr.size();i++){
			draw(objarr[i],norm);
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
