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

template <class T>
struct Node{
	T dat;
	Node* next;
	Node* last;
	Node(T d){dat=d;next=NULL;last=NULL;}
};
class List{
	int count=0;

	void create(){
		head=new Node(d);
		current=&head;
		end=current;
		count=1;
	}
	public:
	Node<T>* head;
	Node<T>* current;
	Node<T>* end;

	List(T d){
		create;
	}

	void add(T dat){
		if(head==NULL){
			create();
			return;
		}
		Node<T> d(dat);
		end->next=&d;
		d->last=end;
		end=&d;
		++count;
	}

	void delCurrent(){
		if(current==null){//there is no head in the first place
			return;
		}

		Node<T>* tmp=NULL;
		if(current->next==NULL && current->last==NULL){

		}else if(current->next==NULL){
			tmp=current->last;
			tmp->next=NULL;
		}else if(current->last==NULL){
			tmp=current->next;
			tmp->last=NULL;
		}else{
			tmp=current->next;
			tmp->last=current->last;
			tmp=current->last;
			tmp->next=current->next;
		}
		current->last->next=current->next;
		current->next->last=current->last;

		--count;
		delete current;
		current=tmp;
	}


	int size(){return count;}
};

class engine{
	public:
	int depth;
	//consider a linked list instead
	vector<light>* lights;
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

		for(light* l:
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
