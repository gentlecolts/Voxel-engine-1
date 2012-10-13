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

class light:public vCamera{

};

#endif // LIGHTSOURCE_H_INCLUDED
