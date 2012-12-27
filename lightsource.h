#ifndef LIGHTSOURCE_H_INCLUDED
#define LIGHTSOURCE_H_INCLUDED

#include "vCamera.h"

/**Method 1:
 * if going by the model of each lightsource is a camera
 * after getting the lightness of a voxel, divide the lightness by some value or set it to zero
 * the advantages being that the lightness values dont need to be reset
 * and there is either a motion blurring on the shadows too or there is a birhgtness adjusting effect
 * or at a high enough framerate it wont matter
 * also remember to use a voxel pointer and a distance value
 *
 **Method 2:
 * shadows and lighting only need to be changed when an object moves, maybe instead of the above hacky method, use one that takes advantage of this
 * NOTE: lightness might not need to be stored in the voxel nodes themselves, but possably in the object itself
 * NOTE: possably store applicable lightsources and objects in the way
 * NOTE: the ligtsource could have a trees or lists, each starts with an object that has nothing between it and the lightsource, to build the structure, go along each vector from the lightsource and add by distance/order of intersection
 *
 **Method 3(current):
 * use a spherical coordinate zbuffer
 * note: quadratic (or cosine) interpolation is a good idea here
 * note: tolerance is prob best, though...
 * note: also could have a falloff, the greater the difference between the disntance to the voxel and the stored value, the darker the color returned
 * note: for at least initial testing: could use only the distance to the root voxel?
 */

class light{
	int res;//how many angles there are, recalculation necessary on change
	public:
	const static int def_res=20;
	double zbuf[];//might need a list? needs to deal with semitransparency
	int range;//doesnt need this? can speed up but might not need to
	//also see http://www.gamedev.net/topic/386146-opengl-attenuation-settings/

	light(int resolution=def_res){
		zbuf[def_res*def_res];
	}
};

#endif // LIGHTSOURCE_H_INCLUDED
