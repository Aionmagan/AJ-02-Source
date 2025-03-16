#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "matrix.h"
#include "vector.h"
#include "quaternion.h" 

class Camera
{
public: 
	Camera(); 
   ~Camera(); 
   
    Vector position; 
    Quaternion rotation; 
   	
   	void applyView();
   	void lookAt(Vector & target); 
   
private:
	Matrix r;
	Matrix t;  
	
public:
	static Matrix view; 
	static Matrix invView; 
};

#endif/*__CAMERA_H__*/
