#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "quaternion.h"
#include "common.h" 

struct Quaternion; 

struct Vector
{
	union
	{
		float elem[4];
		struct
		{	
			float x, y, z, w; 
		};
	}AJ_ALIGNED;
	
	Vector(float xx, float yy, float zz, float ww);
	Vector(float xx, float yy, float zz);
	Vector(float scale);  
	Vector();
   ~Vector();  
   
    float magnitude();
    float sqrMagnitude();  
    Vector normalized(); 
    void normalize(); 
	
	bool     operator == (const Vector & v);
	Vector & operator -= (const Vector & v);
	Vector & operator += (const Vector & v);
	Vector & operator *= (const float s); 
	Vector & operator /= (const float s);  
	
	/*======================================
	 *Static Function / Memebers
	 *=====================================*/
	
	/*MAKE GLOBAL FUNCTION STATIC INSTEAD*/
	static Vector Cross      (const Vector & v1, const Vector & v2);
	static float  SqrDistance(const Vector & v1, const Vector & v2); 
	static float  Distance   (const Vector & v1, const Vector & v2); 
    static float  Dot        (const Vector & v1, const Vector & v2);
	
	static Vector Normalize  (const Vector & v); 
	static Vector Lerp       (const Vector & v1, const Vector & v2, float l);
	static void OrthoNormalize(Vector & v1, Vector & v2); 
	static Vector MoveTowards(const Vector & v1, const Vector & v2, float l); 	
	
    //adds W into the equation
    static float  Dot4(const Vector & v1, const Vector & v2);
    
    static Vector Forward();
    static Vector Up(); 
    static Vector Right();
    static Vector Zero(); 
    static Vector One();   
};

/*
 *=============================================
 *Global Vector functions
 *=============================================
 */
 
 Vector operator - (const Vector & v);
 Vector operator - (const Vector & v1, const Vector & v2);
 Vector operator + (const Vector & v1, const Vector & v2);
 Vector operator * (const Vector & v, const float s);
 Vector operator * (const Quaternion & q, const Vector & v);
 Vector operator * (const float s, const Vector & v);
 Vector operator / (const Vector & v, const float s); 
 Vector operator / (const float s, const Vector & v); 
 
#endif/*__VECTOR_H__*/
