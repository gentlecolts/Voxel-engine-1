#ifndef VNODE_H_INCLUDED
#define VNODE_H_INCLUDED

#include "miscClasses.h"

struct vNode{//no transparency, 8bit rgb
	vNode* children[8];
	//uint16_t shape;//8 flags for which children exist and which are leaf nodes
	uint8_t shape;//8 flags for which children exist, but is this really necessary?
	static const uint8_t a=255;
	uint8_t r,g,b;
	//uint8_t light;//no longer needed due to new algorigthm

	~vNode(){delete[] children;}
	operator uint32_t(){
		//return (((uint32_t)light)<<24)|(((uint32_t)b)<<16)|(((uint32_t)g)<<8)|((uint32_t)r);
		//return 0xff000000|(((uint32_t)b)<<16)|(((uint32_t)g)<<8)|((uint32_t)r);
		return (((uint32_t)a)<<24)|(((uint32_t)b)<<16)|(((uint32_t)g)<<8)|((uint32_t)r);
	}
};

struct vNodeAlpha:vNode{//alpha channel
	vNode* children[8];
	//uint16_t shape;//8 flags for which children exist and which are leaf nodes
	uint8_t shape;//8 flags for which children exist, do i need this?
	uint8_t r,g,b,a;
	//uint8_t light;

	~vNodeAlpha(){delete[] children;}

	//llllllll aaaaaaaa 00000000bbbbbbbb 00000000gggggggg 00000000rrrrrrrr
	//aaaaaaaa bbbbbbbb gggggggg rrrrrrrr
	operator uint32_t(){
		//return (((uint64_t)light)<<56)|(((uint64_t)a)<<48)|(((uint64_t)b)<<32)|(((uint64_t)g)<<16)|((uint64_t)r);
		return (((uint32_t)a)<<24)|(((uint32_t)b)<<16)|(((uint32_t)g)<<8)|((uint32_t)r);
	}
};

struct vNode16:vNode{//16bit rgb
	vNode *children[8];
	//uint16_t shape;//8 flags for which children exist and which are leaf nodes
	uint8_t shape;//8 flags for which children exist
	uint16_t r,g,b;
	uint16_t a;
	//uint8_t light;

	~vNode16(){delete[] children;}

	//llllllll aaaaaaaa bbbbbbbbbbbbbbbb gggggggggggggggg rrrrrrrrrrrrrrrr
	//aaaaaaaaaaaaaaaa bbbbbbbbbbbbbbbb gggggggggggggggg rrrrrrrrrrrrrrrr
	operator uint64_t(){
		//return (((uint64_t)light)<<56)|(((uint64_t)a)<<48)|(((uint64_t)b)<<32)|(((uint64_t)g)<<16)|((uint64_t)r);
		return (((uint64_t)a)<<48)|(((uint64_t)b)<<32)|(((uint64_t)g)<<16)|((uint64_t)r);
	}
};

#endif // VNODE_H_INCLUDED
