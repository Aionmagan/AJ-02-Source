#include "matrix.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <dc/matrix.h>
#include <dc/matrix3d.h>
#include <dc/fmath.h>
#include "common.h" 

Matrix::Matrix(float m11, float m12, float m13, float m14,
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34,
               float m41, float m42, float m43, float m44)
{
    elem[0][0] = m11; elem[0][1] = m12; elem[0][2] = m13; elem[0][3] = m14; 
    elem[1][0] = m21; elem[1][1] = m22; elem[1][2] = m23; elem[1][3] = m24; 
    elem[2][0] = m31; elem[2][1] = m32; elem[2][2] = m33; elem[2][3] = m34;
    elem[3][0] = m41; elem[3][1] = m42; elem[3][2] = m43; elem[3][3] = m44; 
}               
        
Matrix::Matrix(const Matrix & m)
{
    (*this) = m; 
}
 
Matrix::Matrix()
{
    identity(); 
}
 
Matrix::~Matrix()
{

}

void Matrix::identity()
{
    elem[0][0] = 1.0f; elem[0][1] = 0.0f; elem[0][2] = 0.0f; elem[0][3] = 0.0f; 
    elem[1][0] = 0.0f; elem[1][1] = 1.0f; elem[1][2] = 0.0f; elem[1][3] = 0.0f; 
    elem[2][0] = 0.0f; elem[2][1] = 0.0f; elem[2][2] = 1.0f; elem[2][3] = 0.0f;
    elem[3][0] = 0.0f; elem[3][1] = 0.0f; elem[3][2] = 0.0f; elem[3][3] = 1.0f;  
}

void Matrix::translate(float x, float y, float z)
{
    identity(); 
    
    //row major need to change
    elem[3][0] = x; 
    elem[3][1] = y; 
    elem[3][2] = z; 

	//elem[0][3] = x; 
    //elem[1][3] = y; 
    //elem[2][3] = z; 
	
    //elem[12] = x; 
    //elem[13] = y; 
    //elem[14] = z;  
}

void Matrix::scale(float x, float y, float z)
{
    identity(); 
    
    elem[0][0] = x; 
    elem[1][1] = y; 
    elem[2][2] = z; 
    
    //elem[0 ] = x; 
    //elem[5 ] = y;
    //elem[10] = z;  
}

/*all rotation uses radians*/
void Matrix::rotate(float x, float y, float z)
{
    //doesn't work because it's overwritting all matrices
    //should multiply
    //rotateZ(z);
    //rotateY(y); 
    //rotateX(x); 
    
    float xc = cosf(x); 
    float xs = sinf(x); 
    float yc = cosf(y); 
    float ys = sinf(y); 
    float zc = cosf(z); 
    float zs = sinf(z); 
    
    
    //elem[0][0] = yc*zc; 
    //elem[1][1] = xc*zc - xs*ys*zs; 
    //elem[2][2] = xc*yc; 
    
    //elem[0][1] = -(yc*zs);
    //elem[0][2] =  ys; 
    //elem[1][0] =  zc*xs*ys + xc*zs; 
    //elem[1][2] = -(yc*xs); 
    //elem[2][0] = -(xc*zc*ys + xs*ys); 
    //elem[2][1] =  zc*xs + xc*ys*zs; 
    
    /*this is in XYZ order, need to change it later to ZYX
     *but i'm too tired now
     */
     

}
 
void Matrix::rotateX(float radian)
{
    identity(); 
    
    float c = 0.0f; 
    float s = 0.0f; 
    fsincosr(radian, &s, &c);
    
    elem[1][1] =  c; 
    elem[1][2] =  s;
    elem[2][1] = -s;
    elem[2][2] =  c; 
    //elem[5 ] =  c; 
    //elem[6 ] = -s; 
    //elem[9 ] =  s; 
    //elem[10] =  c; 
}

void Matrix::rotateY(float radian)
{
    identity(); 
    
    float c = 0.0f; 
    float s = 0.0f; 
    fsincosr(radian, &s, &c);
    
    elem[0][0] =  c; 
    elem[0][2] =  s;
    elem[2][0] = -s;
    elem[2][2] =  c; 
    
    //elem[0 ] =  c; 
    //elem[2 ] =  s; 
    //elem[8 ] = -s; 
    //elem[10] =  c; 
}

void Matrix::rotateZ(float radian)
{
    identity(); 
    
    float c = 0.0f; 
    float s = 0.0f; 
    fsincosr(radian, &s, &c);
    
    elem[0][0] =  c; 
    elem[0][1] =  s;
    elem[1][0] = -s;
    elem[1][1] =  c; 
    
    //elem[0 ] =  c;
    //elem[1 ] = -s; 
    //elem[4 ] =  s;
    //elem[5 ] =  c; 
}

/*================================
 *Vector parameters
 *===============================*/
void Matrix::translate(Vector & v)
{
    identity(); 
    
    elem[3][0] = v.x; 
    elem[3][1] = v.y; 
    elem[3][2] = v.z; 
}

void Matrix::scale(Vector & v)
{
    identity(); 
    
    elem[0][0] = v.x; 
    elem[1][1] = v.y; 
    elem[2][2] = v.z; 
}

void Matrix::rotate(Vector & v)
{
    rotate(v.x, v.y, v.z); 
}

void Matrix::transpose()
{
    Matrix t(*this); 
    
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            elem[i][j] = t(j,i); 
            //elem[i+j*4] = t.elem[j+i*4];
        }
    }
}

void Matrix::lookAt(Vector & eye, Vector & target, Vector & up)
{
    Vector zAxis = (eye - target).normalized();
    Vector xAxis = Vector::Cross(up, zAxis).normalized();
    Vector yAxis = Vector::Cross(zAxis, xAxis).normalized();

	elem[0][0] = xAxis.x; elem[0][1] = yAxis.x; elem[0][2] = zAxis.x; elem[0][3] = 0;
	elem[1][0] = xAxis.y; elem[1][1] = yAxis.y; elem[1][2] = zAxis.y; elem[1][3] = 0;
	elem[2][0] = xAxis.z; elem[2][1] = yAxis.z; elem[2][2] = zAxis.z; elem[2][3] = 0;
	
	elem[3][0] = -xAxis.x * eye.x - xAxis.y * eye.y - xAxis.z * eye.z;
	elem[3][1] = -yAxis.x * eye.x - yAxis.y * eye.y - yAxis.z * eye.z;
	elem[3][2] = -zAxis.x * eye.x - zAxis.y * eye.y - zAxis.z * eye.z;
	
	elem[3][3] = 1;
}

void Matrix::perspectiveProj(float fov, float aspect, float near, float far)
{
	float w = tanf(fov*3.14159f/180.0f)*0.5f;
	float h = w/aspect;
	float x = 0.0f;
	float y = 0.0f;
	
	matrix_t screen __attribute__((aligned(8)))= 
	{
		{320.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 240.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{320.0f, 240.0f, 0.0f, 1.0f}
	};

	matrix_t proj __attribute__((aligned(8)))= 
	{
		{1.0f/w,   0.0f, 0.0f, 0.0f},
		{  0.0f, 1.0f/h, 0.0f, 0.0f},
		{   x/w,    y/h, (far+near)/(far-near), 1.0f},
		{  -x/w,   -y/h, -2.0f*near*far/(far-near), 1.0f},
	};

    mat_identity();
    mat_identity();
    //mat_perspective(320, 240,fh, 0.1f, 1000.0f);
    mat_apply(&screen); 
    mat_apply(&proj); 
    mat_store(&elem);
	mat_identity();
	
	printf("proj created\n");

#if 0   		
   		printf("\n\n");
for(int i = 0; i < 4; ++i)
{	
	for(int j = 0; j < 4; ++j)
		printf("m[%d]=%f | ", i, j, elem[i][j]);
	printf("\n");
}
printf("\n\n");
#endif

}

void Matrix::quaternionToMatrix(const Quaternion & q)
{
#if 1

	float xx = q.x*q.x; 
	float yy = q.y*q.y; 
	float zz = q.z*q.z; 
	float yz = q.y*q.z; 
	float zx = q.z*q.x; 
	float xy = q.x*q.y; 
	float wx = q.w*q.x; 
	float wy = q.w*q.y; 
	float wz = q.w*q.z; 

	identity(); 
#if 0
	/*right*/ 
	elem[0 ] = 1.0f - 2.0f * (yy + zz); 
	elem[4 ] =        2.0f * (xy + wz); 
	elem[8 ] =        2.0f * (zx - wy); 
	
	elem[1 ] =        2.0f * (xy - wz); 
	elem[5 ] = 1.0f - 2.0f * (xx + zz); 
	elem[9 ] =        2.0f * (yz + wx); 
	
	elem[2 ] =        2.0f * (zx + wy); 
	elem[6 ] =        2.0f * (yz - wx);  
	elem[10] = 1.0f - 2.0f * (xx + yy); 
	
	transpose(); 
#endif	
	elem[0][0 ] = 1.0f - 2.0f * (yy + zz); 
	elem[0][1 ] =        2.0f * (xy + wz); 
	elem[0][2 ] =        2.0f * (zx - wy); 
	
	elem[1][0 ] =        2.0f * (xy - wz); 
	elem[1][1 ] = 1.0f - 2.0f * (xx + zz); 
	elem[1][2 ] =        2.0f * (yz + wx); 
	
	elem[2][0 ] =        2.0f * (zx + wy); 
	elem[2][1 ] =        2.0f * (yz - wx);  
	elem[2][2] = 1.0f - 2.0f * (xx + yy); 
	
#endif   
}
    
    
/*=================================
 *static functions and operators 
 *================================*/

Matrix & Matrix::operator = (const Matrix & m)
{
    elem[0][0] = m(0,0); elem[0][1] = m(0,1); elem[0][2] = m(0,2); elem[0][3] = m(0,3);
    elem[1][0] = m(1,0); elem[1][1] = m(1,1); elem[1][2] = m(1,2); elem[1][3] = m(1,3);
    elem[2][0] = m(2,0); elem[2][1] = m(2,1); elem[2][2] = m(2,2); elem[2][3] = m(2,3);
    elem[3][0] = m(3,0); elem[3][1] = m(3,1); elem[3][2] = m(3,2); elem[3][3] = m(3,3);
     
    //elem[0 ] = m.elem[0 ]; elem[1 ] = m.elem[1 ]; elem[2 ] = m.elem[2 ]; elem[3 ] = m.elem[3 ]; 
    //elem[4 ] = m.elem[4 ]; elem[5 ] = m.elem[5 ]; elem[6 ] = m.elem[6 ]; elem[7 ] = m.elem[7 ]; 
    //elem[8 ] = m.elem[8 ]; elem[9 ] = m.elem[9 ]; elem[10] = m.elem[10]; elem[11] = m.elem[11]; 
    //elem[12] = m.elem[12]; elem[13] = m.elem[13]; elem[14] = m.elem[14]; elem[15] = m.elem[15]; 
    return *this; 
}

float & Matrix::operator() (unsigned int r, unsigned int c)
{
    //return elem[r+c*4];
    //return elem[c+r*4]; 
    return elem[c][r];
}

float Matrix::operator() (unsigned int r, unsigned int c)const
{
    //return elem[r+c*4];
    //return elem[c+r*4];
    return elem[c][r];
}  


/*=================================
 * Global operators
 *================================*/
Matrix operator - (const Matrix & m)
{
/*  compiler didn't like this for libs
    return Matrix(-m(0,0), -m(0,1), -m(0,2), -m(0,3),
                  -m(1,0), -m(1,1), -m(1,2), -m(1,3),
                  -m(2,0), -m(2,1), -m(2,2), -m(2,3),
                  -m(3,0), -m(3,1), -m(3,2), -m(3,3)); 
*/
    Matrix mtx(-m(0,0), -m(0,1), -m(0,2), -m(0,3),
               -m(1,0), -m(1,1), -m(1,2), -m(1,3),
               -m(2,0), -m(2,1), -m(2,2), -m(2,3),
               -m(3,0), -m(3,1), -m(3,2), -m(3,3)); 
                  
    return mtx; 
}

Matrix operator - (const Matrix & m1, const Matrix & m2)
{
    Matrix mtx(m1(0,0) - m2(0,0), m1(0,1) - m2(0,1), m1(0,2) - m2(0,2), m1(0,3) - m2(0,3),
	           m1(1,0) - m2(1,0), m1(1,1) - m2(1,1), m1(1,2) - m2(1,2), m1(1,3) - m2(1,3),
	           m1(2,0) - m2(2,0), m1(2,1) - m2(2,1), m1(2,2) - m2(2,2), m1(2,3) - m2(2,3),
	           m1(3,0) - m2(3,0), m1(3,1) - m2(3,1), m1(3,2) - m2(3,2), m1(3,3) - m2(3,3));
	return mtx; 
}

Matrix operator + (const Matrix & m1, const Matrix & m2)
{
    Matrix mtx(m1(0,0) + m2(0,0), m1(0,1) + m2(0,1), m1(0,2) + m2(0,2), m1(0,3) + m2(0,3),
	           m1(1,0) + m2(1,0), m1(1,1) + m2(1,1), m1(1,2) + m2(1,2), m1(1,3) + m2(1,3),
	           m1(2,0) + m2(2,0), m1(2,1) + m2(2,1), m1(2,2) + m2(2,2), m1(2,3) + m2(2,3),
	           m1(3,0) + m2(3,0), m1(3,1) + m2(3,1), m1(3,2) + m2(3,2), m1(3,3) + m2(3,3));
	return mtx; 
}

Matrix operator * (const Matrix & m1, const Matrix & m2)
{
    Matrix result; 
#if 1	    
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {  
            float v = 0; 
            for(int k = 0; k < 4; ++k)
            {
                //v += m1(i,k) * m2(k,j);
                v += m1(k,j) * m2(i,k);
            }
            result(i, j) = v; 
        }
    }
#else

#if 0

	result(0,0) = m2(0,0)*m1(0,0) + m2(1,0)*m1(0,1) + m2(2,0)*m1(0,2) + m2(3,0)*m1(0,3);
	result(1,0) = m2(0,0)*m1(1,0) + m2(1,0)*m1(1,1) + m2(2,0)*m1(1,2) + m2(3,0)*m1(1,3);
	result(2,0) = m2(0,0)*m1(2,0) + m2(1,0)*m1(2,1) + m2(2,0)*m1(2,2) + m2(3,0)*m1(2,3);
	result(3,0) = m2(0,0)*m1(3,0) + m2(1,0)*m1(3,1) + m2(2,0)*m1(3,2) + m2(3,0)*m1(3,3);
	
	result(0,1) = m2(0,1)*m1(0,0) + m2(1,1)*m1(0,1) + m2(2,1)*m1(0,2) + m2(3,1)*m1(0,3);
	result(1,1) = m2(0,1)*m1(1,0) + m2(1,1)*m1(1,1) + m2(2,1)*m1(1,2) + m2(3,1)*m1(1,3);
	result(2,1) = m2(0,1)*m1(2,0) + m2(1,1)*m1(2,1) + m2(2,1)*m1(2,2) + m2(3,1)*m1(2,3);
	result(3,1) = m2(0,1)*m1(3,0) + m2(1,1)*m1(3,1) + m2(2,1)*m1(3,2) + m2(3,1)*m1(3,3);
	
	result(0,2) = m2(0,2)*m1(0,0) + m2(1,2)*m1(0,1) + m2(2,2)*m1(0,2) + m2(3,2)*m1(0,3);
	result(1,2) = m2(0,2)*m1(1,0) + m2(1,2)*m1(1,1) + m2(2,2)*m1(1,2) + m2(3,2)*m1(1,3);
	result(2,2) = m2(0,2)*m1(2,0) + m2(1,2)*m1(2,1) + m2(2,2)*m1(2,2) + m2(3,2)*m1(2,3);
	result(3,2) = m2(0,2)*m1(3,0) + m2(1,2)*m1(3,1) + m2(2,2)*m1(3,2) + m2(3,2)*m1(3,3);

	result(0,3) = m2(0,3)*m1(0,0) + m2(1,3)*m1(0,1) + m2(2,3)*m1(0,2) + m2(3,3)*m1(0,3);
	result(1,3) = m2(0,3)*m1(1,0) + m2(1,3)*m1(1,1) + m2(2,3)*m1(1,2) + m2(3,3)*m1(1,3);
	result(2,3) = m2(0,3)*m1(2,0) + m2(1,3)*m1(2,1) + m2(2,3)*m1(2,2) + m2(3,3)*m1(2,3);
	result(3,3) = m2(0,3)*m1(3,0) + m2(1,3)*m1(3,1) + m2(2,3)*m1(3,2) + m2(3,3)*m1(3,3);
	
#else

	result(0,0) = m1(0,0)*m2(0,0) + m1(0,1)*m2(1,0) + m1(0,2)*m2(2,0) + m1(0,3)*m2(3,0);
	result(0,1) = m1(0,0)*m2(0,1) + m1(0,1)*m2(1,1) + m1(0,2)*m2(2,1) + m1(0,3)*m2(3,1);
	result(0,2) = m1(0,0)*m2(0,2) + m1(0,1)*m2(1,2) + m1(0,2)*m2(2,2) + m1(0,3)*m2(3,2);
	result(0,3) = m1(0,0)*m2(0,3) + m1(0,1)*m2(1,3) + m1(0,2)*m2(2,3) + m1(0,3)*m2(3,3);
	
	result(1,0) = m1(1,0)*m2(0,0) + m1(1,1)*m2(1,0) + m1(1,2)*m2(2,0) + m1(1,3)*m2(3,0);
	result(1,1) = m1(1,0)*m2(0,1) + m1(1,1)*m2(1,1) + m1(1,2)*m2(2,1) + m1(1,3)*m2(3,1);
	result(1,2) = m1(1,0)*m2(0,2) + m1(1,1)*m2(1,2) + m1(1,2)*m2(2,2) + m1(1,3)*m2(3,2);
	result(1,3) = m1(1,0)*m2(0,3) + m1(1,1)*m2(1,3) + m1(1,2)*m2(2,3) + m1(1,3)*m2(3,3);
	
	result(2,0) = m1(2,0)*m2(0,0) + m1(2,1)*m2(1,0) + m1(2,2)*m2(2,0) + m1(2,3)*m2(3,0);
	result(2,1) = m1(2,0)*m2(0,1) + m1(2,1)*m2(1,1) + m1(2,2)*m2(2,1) + m1(2,3)*m2(3,1);
	result(2,2) = m1(2,0)*m2(0,2) + m1(2,1)*m2(1,2) + m1(2,2)*m2(2,2) + m1(2,3)*m2(3,2);
	result(2,3) = m1(2,0)*m2(0,3) + m1(2,1)*m2(1,3) + m1(2,2)*m2(2,3) + m1(2,3)*m2(3,3);

	result(2,0) = m1(2,0)*m2(0,0) + m1(2,1)*m2(1,0) + m1(2,2)*m2(2,0) + m1(2,3)*m2(3,0);
	result(2,1) = m1(2,0)*m2(0,1) + m1(2,1)*m2(1,1) + m1(2,2)*m2(2,1) + m1(2,3)*m2(3,1);
	result(2,2) = m1(2,0)*m2(0,2) + m1(2,1)*m2(1,2) + m1(2,2)*m2(2,2) + m1(2,3)*m2(3,2);
	result(2,3) = m1(2,0)*m2(0,3) + m1(2,1)*m2(1,3) + m1(2,2)*m2(2,3) + m1(2,3)*m2(3,3);
#endif

#endif

	return result; 
}

Matrix operator * (const Matrix & m, const float s)
{
    Matrix mtx(m(0,0) * s, m(0,1) * s, m(0,2) * s, m(0,3) * s,
	           m(1,0) * s, m(1,1) * s, m(1,2) * s, m(1,3) * s,
	           m(2,0) * s, m(2,1) * s, m(2,2) * s, m(2,3) * s,
	           m(3,0) * s, m(3,1) * s, m(3,2) * s, m(3,3) * s);
	return mtx; 
}

Matrix operator * (const float s, const Matrix & m)
{
    Matrix mtx(m(0,0) * s, m(0,1) * s, m(0,2) * s, m(0,3) * s,
	           m(1,0) * s, m(1,1) * s, m(1,2) * s, m(1,3) * s,
	           m(2,0) * s, m(2,1) * s, m(2,2) * s, m(2,3) * s,
	           m(3,0) * s, m(3,1) * s, m(3,2) * s, m(3,3) * s);
	return mtx; 
}

Matrix Transpose(const Matrix & m)
{
    Matrix t; 
    
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            t(i,j) = m(j,i); 
        }
    }
    
    return t; 
}

