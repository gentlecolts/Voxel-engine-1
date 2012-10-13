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
#include <vWindow.h>//TODO, remake window for this program, other one is a mess
#undef SDL

#include "lightsource.h"
//#include <cstdlib>

class engine{
	public:
	int depth;
	vector<vObj> objects;
	vStatic scene;
	vCamera camera;

	int px,py;

	/**create the window
	 * create the camera
	 * set the camera pointer in vobj to the camera
	 */
	engine(){
	}

	void flashsort(vector<vObj*> arr,int len){//TODO change to vObj array, vObj needs a double cast defined as the distance^2 from the camera
		//CLASS FORMATION
		double amin=*arr[0];
		int nmax=1;
		const int m=0.42*len;
		int L[m];
		for(int i=0;i<len;i++){
			 if(*arr[i]<amin){amin=*arr[i];}
			 if(*arr[i]>*arr[nmax]){nmax=i+1;}
		}
		if(amin==*arr[nmax-1]){return;}
		double C1=(m-1)/(*arr[nmax-1]-amin);

		for(int k=0;k<m;k++){
			L[k]=0;
		}
		int K;
		for(int i=0;i<len;i++){
			 K=1+(int)(C1*(*arr[i]-amin));
			 L[K-1]=L[K-1]+1;
		}
		for(int k=2;k<=m;k++){
			 L[k-1]=L[k-1] + L[k-2];
		}
		vObj *tmp=arr[nmax-1];
		arr[nmax-1]=arr[0];
		arr[0]=tmp;
		//PERMUTATION
		double NMOVE=0;
		int J=1;
		K=m;
		vObj* FLASH;
		while(NMOVE<len - 1){
			 while(J>L[K-1]){
				++J;
				K=1+(int)(C1*(*arr[J-1]-amin));
			 }
			 FLASH=arr[J-1];
			 while(J!=L[K-1]+1){
				K=1+(int)(C1*(*FLASH - amin));
				tmp=arr[L[K-1]-1];
				arr[L[K-1]-1]=FLASH;
				FLASH=tmp;
				L[K-1]=L[K-1] - 1;
				++NMOVE;
			 }
		}
		//STRAIGHT INSERTION
		for(int i=len-2;i>=1;i--){
			 if(arr[i]<arr[i-1]){
				tmp=arr[i-1];
				J=i;
				while(arr[J]<tmp){
				   arr[J-1]=arr[J];
				   ++J;
				}
				arr[J-1]=tmp;
			 }
		}
	}

	/**sort visible objects by distance to camera
	 * have the camera draw them
	 * apply post-processing filters
	 */
	void drawScene(){
		vObj::cam=camera;
		vector<vObj*> t;
		for(int i=0;i<objects.size();i++){
			if(camera.canSee(objects[i])){
				t.push_back(&objects[i]);
			}
		}

		pixel parr[px*py];
		camera.rendView(px,py,t,&scene,parr);
	}

	/**format:
	 * vox
	 * ascii version number ended with ";" (ex vox123;)
	 * data:
	 *
	*/
	void readFile(string path){
	}
};
#endif // ENGINE_H_INCLUDED
