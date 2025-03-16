#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"
#include "quaternion.h"
#include "common.h" 
#include <dc/matrix.h>

struct Matrix
{
    Matrix(float m11, float m12, float m13, float m14,
           float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34,
           float m41, float m42, float m43, float m44);
            
    Matrix(const Matrix & m); 
    Matrix(); //identity
   ~Matrix(); 
   
    void identity(); 
   
    void translate(float x, float y, float z); 
    void scale(float x, float y, float z);
    
    /*all rotation uses radians*/
    /*rotate function is Z Y X order*/
    void rotate(float x, float y, float z);  
    void rotateX(float radian);
    void rotateY(float radian);
    void rotateZ(float radian);
    
    void transpose(); 
    
/*================================
 *Vector parameters
 *===============================*/
    void translate(Vector & v);
    void scale(Vector & v);  
    void rotate(Vector & v);  
    
    void lookAt(Vector & eye, Vector & target, Vector & up);
    void perspectiveProj(float fov, float aspect, float near, float far);  
    void quaternionToMatrix(const Quaternion & q); 
    
/*=================================
 *static functions and operators 
 *================================*/
    Matrix & operator = (const Matrix & m);
    float  & operator() (unsigned int r, unsigned int c);
    float    operator() (unsigned int r, unsigned int c)const;  
    
//private:
    //float elem[4][4]; //elements
public: 

    //typedef __attribute__ ((aligned (32))) float matrix_t[4][4];
    matrix_t elem;
};


/*=================================
 * Global operators / function
 *================================*/
Matrix operator - (const Matrix & m);
Matrix operator - (const Matrix & m1, const Matrix & m2);
Matrix operator + (const Matrix & m1, const Matrix & m2);
Matrix operator * (const Matrix & m1, const Matrix & m2);
Matrix operator * (const Matrix & m, const float s);
Matrix operator * (const float s, const Matrix & m);

//make this a static function later
Matrix Transpose(const Matrix & m); 

#endif/*__MATRIX_H__*/
