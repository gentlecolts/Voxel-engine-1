#ifndef MISCCLASSES_H_INCLUDED
#define MISCCLASSES_H_INCLUDED

#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits.h>
#include <stdint.h>
#include <string>

using namespace std;

struct pixel{
	/**needs:
	* rgb as double (0-1)
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

#endif // MISCCLASSES_H_INCLUDED
