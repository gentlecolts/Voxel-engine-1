#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

/**incorporate this into vWind
#define renderer 0//0=sdl,
#if renderer=0
#include <SDL/SDL.h>
#else
#endif
*/

#define SDL
//#include <vWindow.h>//TODO, remake window for this program, other one is a mess
#undef SDL

#include "lightsource.h"
//#include <cstdlib>

class engine{
	public:
	int depth;
	vector<vObj>* objects;//these are pointers to allow the user of this libary to separatly load and set the objects of the current map
	vStatic* scene;       //allowing for quick swapping and the use of threads as memory allows
	vCamera camera;///figure out how to initialize here

	int px,py;

	/**create the window
	 * create the camera
	 */
	engine(){
	}

	/**have the camera draw objects
	 * apply post-processing filters, try to make it as modular as possable, how can the programmer implement their own?
	 */
	void drawScene(){
		/*
		vObj::cam=camera;
		vector<vObj*> t;
		for(int i=0;i<objects.size();i++){
			if(camera.canSee(objects[i])){
				t.push_back(&objects[i]);
			}
		}//*/

		pixel parr[px*py];//because otherwise rendview would have to know how big the screen is
		camera.rendView(px,py,objects,scene,parr);
	}

	/**format:
	 * vox
	 * ascii version number ended with ";" (ex vox123;)
	 * data: read as if it were a heap, but the information stored in vnode's shape will define the tree
	 *
	*/
	//TODO, figure out the file format
	void readFile(string path){
	}
};
#endif // ENGINE_H_INCLUDED
