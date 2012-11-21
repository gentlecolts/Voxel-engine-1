#ifndef LIGHTSOURCE_H_INCLUDED
#define LIGHTSOURCE_H_INCLUDED

#include "vCamera.h"

/**note to self: if going by the model of each lightsource is a camera
 * after getting the lightness of a voxel, divide the lightness by some value or set it to zero
 * the advantages being that the lightness values dont need to be reset
 * and there is either a motion blurring on the shadows too or there is a birhgtness adjusting effect
 * or at a high enough framerate it wont matter
 * also remember to use a voxel pointer and a distance value
 */

///NOTE: shadows and lighting only need to be changed when an object moves, maybe instead of the above hacky method, use one that takes advantage of this
///NOTE2: lightness might not need to be stored in the voxel nodes themselves, but possably in the object itself
///NOTE2.1: possably store applicable lightsources and objects in the way
///NOTE2.2: the ligtsource could have a trees or lists, each starts with an object that has nothing between it and the lightsource, to build the structure, go along each vector from the lightsource and add by distance/order of intersection
class light:public vCamera{

};

#endif // LIGHTSOURCE_H_INCLUDED
