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
template <class T>
class List{
	int count;

	void reset(T d){
		head=new Node<T>(d);
		current=&head;
		end=current;
		count=1;
	}
	public:
	Node<T>* head;
	Node<T>* current;
	Node<T>* end;

	List(T d){
		reset(d);
	}

	void add(T dat){
		if(head==NULL){
			reset(dat);
			return;
		}
		Node<T> d(dat);
		end->next=&d;
		d->last=end;
		end=&d;
		++count;
	}

	void delCurrent(){
		if(current==NULL){//there is no head in the first place
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
	int depth;///what was this for?
	//consider a linked list instead
	vector<vLight> lights;
	vector<vObj> objects;
	vObj* scene;
	vCamera camera;///figure out how to initialize here (if possable)

	static bool doPost;//do post processing filters

	int px,py;

	/**create the window
	 * create the camera
	 */
	engine(){
	}

	/**calculate lighting
	 * have the camera draw objects
	 * apply post-processing filters
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

		const int len=lights.size();
		for(int i=0;i<len;i++){
			lights[i];///tell it to do something
		}
		pixel parr[px*py];//because otherwise rendview would have to know how big the screen is
		camera.rendView(px,py,&objects,scene,parr);

		if(doPost){//render to an int array, then to the screen
			int final[px*py];///consider a more direct final medium (ex. sdl's screen variable)
			post(parr,final);
		}else{//render from parr to the screen
		}
	}

	void post(pixel pix[],

	/**format:
	 * vox
	 * ascii version number ended with ";" (ex vox123;)
	 * data: read as if it were a heap, but the information stored in vnode's shape will define the tree
	 * alternate: store 8bit shape var, do prefix, if a byte is 0x00, then the next values are color data
	*/
	///TODO, figure out the file format
	void readFile(string path){
	}
};

bool engine::doPost=true;
#endif // ENGINE_H_INCLUDED
