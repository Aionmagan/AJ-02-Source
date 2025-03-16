#include "vector.h"
#include <cmath>
#include <dc/fmath.h>

/*
 *==================================================
 * Local functions and constructor / destrutor 
 *==================================================
 */
 
Vector::Vector(float xx, float yy, float zz, float ww)
{
	x = xx; y = yy; z = zz; w = ww; 
}

Vector::Vector(float xx, float yy, float zz)
{
	x = xx; y = yy; z = zz; w = 1.0f; 
}

Vector::Vector(float scale)
{
	x = scale; y = scale; z = scale; w = 1.0f; 
}


Vector::Vector()
{
	x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f; 
}

Vector::~Vector()
{

}
   
float Vector::magnitude()
{
	return fsqrt(fipr_magnitude_sqr(x, y, z, 0)); 
}

float Vector::sqrMagnitude()
{
	return fipr_magnitude_sqr(x, y, z, 0);//(x*x)+(y*y)+(z*z); 
}

Vector Vector::normalized()
{
	float l = magnitude(); 
	return Vector(x/l, y/l, z/l); 
}

void Vector::normalize()
{
	float l = magnitude(); 
	x = x/l;
	y = y/l; 
	z = z/l; 
	//return Vector(x/l, y/l, z/l); 
}
	
bool Vector::operator == (const Vector & v)
{
	return (x==v.x) && (y==v.y) && (z==v.z);
}

Vector & Vector::operator -= (const Vector & v)
{
	x -= v.x; y -= v.y; z -= v.z;
	w = 1.0f; 
	return *this; 
}

Vector & Vector::operator += (const Vector & v)
{
	x += v.x; y += v.y; z += v.z;
	w = 1.0f; 
	return *this;
}

Vector & Vector::operator *= (const float s)
{
	x *= s; y *= s; z *= s;
	w = 1.0f; 
	return *this; 
}
 
Vector & Vector::operator /= (const float s)
{
	x /= s; y /= s; z /= s;
	w = 1.0f; 
	return *this; 
}



/*======================================
 *Static Function / Memebers
 *=====================================*/
	
Vector Vector::Cross(const Vector & v1, const Vector & v2)
{
	return Vector(v1.y*v2.z - v1.z*v2.y,
	              v1.z*v2.x - v1.x*v2.z,
	              v1.x*v2.y - v1.y*v2.x); 
}

float Vector::SqrDistance(const Vector & v1, const Vector & v2)
{
	return ((v1.x - v2.x)*(v1.x - v2.x)) + 
	       ((v1.y - v2.y)*(v1.y - v2.y)) + 
	       ((v1.z - v2.z)*(v1.z - v2.z));
}

float Vector::Distance(const Vector & v1, const Vector & v2)
{
	return fsqrt(((v1.x - v2.x)*(v1.x - v2.x)) + 
	       		((v1.y - v2.y)*(v1.y - v2.y)) + 
	       		((v1.z - v2.z)*(v1.z - v2.z)));
}

float Vector::Dot(const Vector & v1, const Vector & v2)
{
	return fipr(v1.x, v1.y, v1.z, 0, v2.x, v2.y, v2.z, 0); //(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); 
}

Vector Vector::Normalize(const Vector & v)
{
	Vector t = v; 
	float l = t.magnitude(); 
	return Vector(t.x/l, t.y/l, t.z/l); 
}

Vector Vector::Lerp(const Vector & v1, const Vector & v2, float l)
{
	//Vector output; 
	
	//output = v1 + (v2 - v1);
	return Vector(v1 + l * (v2 - v1));
}

void Vector::OrthoNormalize(Vector & v1, Vector & v2)
{
	Vector f = Vector::Cross(v1.normalized(), v2); 
	v2 = Vector::Cross(f.normalized(), v1.normalized());  
	
	v1 = f; 
}

Vector Vector::MoveTowards(const Vector & v1, const Vector & v2, float l)
{
    Vector a; 
    float mag; 
    a = v2 - v1; 
    mag = a.magnitude();
    
    if (mag <= l || mag == 0.0f)
    	return Vector(v2); 
    
	
	return Vector(v1.x + a.x / mag * l,
	              v1.y + a.y / mag * l,
	              v1.z + a.z / mag * l);
}

float Vector::Dot4(const Vector & v1, const Vector & v2)
{
	return fipr(v1.x, v1.y, v1.z, v1.w, v2.x, v2.y, v2.z, v2.w);
}

Vector Vector::Forward()
{
    return Vector(1.0f, 0.0f, 0.0f); 
}

Vector Vector::Up()
{
    return Vector(0.0f, 1.0f, 0.0f); 
}

Vector Vector::Right()
{
    return Vector(0.0f, 0.0f, 1.0f); 
}

Vector Vector::Zero()
{
    return Vector(0.0f, 0.0f, 0.0f); 
}

Vector Vector::One()
{
    return Vector(1.0f, 1.0f, 1.0f); 
}

/*
 *==================================================
 * Global functions 
 *==================================================
 */
 
Vector operator - (const Vector & v)
{
	return Vector(-v.x, -v.y, -v.z); 
}

Vector operator - (const Vector & v1, const Vector & v2)
{
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); 
}

Vector operator + (const Vector & v1, const Vector & v2)
{
	return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); 
}

Vector operator * (const Vector & v, const float s)
{
	return Vector(v.x * s, v.y * s, v.z * s); 
}

Vector operator * (const Quaternion & q, const Vector & v)
{
    Quaternion p(v.x, v.y, v.z, 0.0f);
    
    p = q * p * Quaternion::Conjugate(q);  
    
    return Vector(p.x, p.y, p.z); 
}

Vector operator * (const float s, const Vector & v)
{
	return Vector(v.x * s, v.y * s, v.z * s); 
}

Vector operator / (const Vector & v, const float s)
{
	return Vector(v.x / s, v.y / s, v.z / s); 
}

Vector operator / (const float s, const Vector & v)
{
	return Vector(v.x / s, v.y / s, v.z / s); 
}
