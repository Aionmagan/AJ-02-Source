#include "camera.h"

#include "common.h"
#include <cmath>
#include <dc/math.h>
#include <dc/matrix.h>

Matrix Camera::view;
Matrix Camera::invView; 

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::applyView()
{
	t.translate(-position.x,
	            -position.y,
	            -position.z);
	//t.transpose();               
	r.quaternionToMatrix(rotation); 

	mat_identity();
	mat_apply(&r.elem); 
	mat_apply(&t.elem);
	mat_store(&view.elem);
	mat_identity();

	view.elem[0][1]  = -view.elem[0][1];
	view.elem[1][1]  = -view.elem[1][1];
	view.elem[2][1]  = -view.elem[2][1];
	view.elem[3][1]  = -view.elem[3][1];

	//r.transpose(); 
	
	//invView = r; 
}

void Camera::lookAt(Vector & target)
{
	Vector dir = target - position; 
	
	if (dir == Vector::Zero()) return; 
	
	dir = Vector::Normalize(-dir);
	float x = RAD2DEG(-asinf(dir.y)); 
	float y = RAD2DEG(-atan2f(-dir.x, -dir.z)); 
	
	Quaternion q1 = Quaternion::Euler(0, y, 0); 
	Quaternion q = Quaternion::Euler(x, 0, 0) * q1;
	
	rotation = q;  
}

