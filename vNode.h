#ifndef VNODE_H_INCLUDED
#define VNODE_H_INCLUDED

#include <vector>

class vNode{
	public:
	vNode *children[8];//these also serve as the pointers to its children
	uint16_t shape;//8 flags for which children exist and which are leaf nodes
	uint32_t color;//rgba
	uint8_t light;

	~vNode(){delete[] children;}
};

#endif // VNODE_H_INCLUDED
