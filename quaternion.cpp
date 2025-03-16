#include "quaternion.h"
#include "common.h" 

#include <cmath>
#include <dc/fmath.h>

/*
 *===================================================
 *Member functions 
 *===================================================
 */
 
Quaternion::Quaternion(float x, float y, float z, float w)
{
    this->x = x; 
    this->y = y; 
    this->z = z; 
    this->w = w; 
}

Quaternion::Quaternion()
{
    x = 0.0f; 
    y = 0.0f; 
    z = 0.0f; 
    w = 1.0f; 
}

Quaternion::~Quaternion()
{
    
}
   
float Quaternion::magnitude()
{
    return fsqrt((w*w)+(x*x)+(y*y)+(z*z));
}

void Quaternion::conjugate()
{
    x = -x; 
    y = -y; 
    z = -z; 
}   

/*float Quaternion::sqrMagnitude()
{
    return ((w*w)+(x*x)+(y*y)+(z*z));
}*/

bool Quaternion::operator == (const Quaternion & q)
{
    
}

/*Quaternion & Quaternion::operator -= (const Quaternion & q)
{
    return Quaternion(x-q.x, y-q.y, z-q.z, w-q.w); 
}

Quaternion & Quaternion::operator += (const Quaternion & q)
{
    return Quaternion(x+q.x, y+q.y, z+q.z, w+q.w); 
}*/

Quaternion & Quaternion::operator *= (const Quaternion & q)
{
    float ww = w*q.w - x*q.x - y*q.y - z*q.z; 
    float xx = w*q.x + x*q.w + y*q.z - z*q.y; 
    float yy = w*q.y + y*q.w + z*q.x - x*q.z;
    float zz = w*q.z + z*q.w + x*q.y - y*q.x; 
    
    w = ww; 
    x = xx; 
    y = yy; 
    z = zz; 
    
    return *this;
}

/*
 *===============================================
 *Static functions 
 *===============================================
 */
 
float Quaternion::Dot(const Quaternion & q1, const Quaternion & q2)
{
    return ((q1.w*q2.w)+(q1.x*q2.x)+(q1.y*q2.y)+(q1.z*q2.z));
}   

Quaternion Quaternion::Conjugate(const Quaternion & q)
{
    return Quaternion(-q.x, -q.y, -q.z, q.w);
}

Quaternion Quaternion::AxisAngle(float x, float y, float z, float angle)
{
	float half = (angle*0.5f) * (3.14159/180.0f); 
	float s = fsin(half); 
	
	return Quaternion(x*s, y*s, z*s, fcos(half)); 
}

void Quaternion::fAxisAngle(Quaternion & q, float x, float y, float z, float angle)
{
	//static float half;
	//static float s;
	
	float half = (angle*0.5f) * (3.14159/180.0f); 
	float s = fsin(half);
	
	q.x= x*s; q.y=y*s; q.z=z*s; q.w=fcos(half);
	//return Quaternion(x*s, y*s, z*s, fcos(half)); 
}

void Quaternion::fEuler(Quaternion & q, float x, float y, float z)
{
	Quaternion qx, qy, qz, qt; 
	Quaternion::fAxisAngle(qx, 1.0f, 0.0f, 0.0f, x); 
	Quaternion::fAxisAngle(qy, 0.0f, 1.0f, 0.0f, y); 
	Quaternion::fAxisAngle(qz, 0.0f, 0.0f, 1.0f, z); 
	//q = (qz*qy); 
	//q *= (qx); 
	Quaternion::fQuadMul(qt, qz, qy);
	Quaternion::fQuadMul(q, qt, qx); 
}

void Quaternion::fQuadMul(Quaternion & out, const Quaternion & q1, const Quaternion & q2)
{
#if 1
#if 0
	out.w = fipr(q1.w, (-q1.x), (-q1.y), (-q1.z), q2.w, q2.x, q2.y, q2.z);
	out.x = fipr(q1.w, q1.x, q1.y, (-q1.z), q2.x, q2.w, q2.z, q2.y);
	out.y = fipr(q1.w, q1.y, q1.z, (-q1.x), q2.y, q2.w, q2.x, q2.z);
	out.z = fipr(q1.w, q1.z, q1.x, (-q1.y), q2.z, q2.w, q2.y, q2.x);
#else
	//out.w = q1.w*q2.w + (-q1.x)*q2.x + (-q1.y)*q2.y + (-q1.z)*q2.z; 
    //out.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z + (-q1.z)*q2.y; 
    //out.y = q1.w*q2.y + q1.y*q2.w + q1.z*q2.x + (-q1.x)*q2.z;
    //out.z = q1.w*q2.z + q1.z*q2.w + q1.x*q2.y + (-q1.y)*q2.x;
    out.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z; 
    out.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y; 
    out.y = q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z;
    out.z = q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x;  
#endif
#else    
    register float __q1w __asm__("fr4") = (q1.w);
    register float __q1x __asm__("fr5") = (-q1.x);
    register float __q1y __asm__("fr6") = (-q1.y);
    register float __q1z __asm__("fr7") = (-q1.z);

	register float __q2w __asm__("fr8") = (q2.w);    
    register float __q2x __asm__("fr9") = (q2.x);
    register float __q2y __asm__("fr10") = (q2.y);
    register float __q2z __asm__("fr11") = (q2.z);

	register float __rx __asm__("fr0") = 0;
    register float __ry __asm__("fr1") = 0;
    register float __rz __asm__("fr2") = 0;
    register float __rw __asm__("fr3") = 0;
#if 0     
     __asm__ volatile("\n"
					"fipr fv4, fv8 \n"
					"fmov fr11, fr0 \n"					
		: "+f" (__rw)
		: "f" (__q1w), "f" (__q1x), "f" (__q1y), "f" (__q1z),
		  "f" (__q2w), "f" (__q2x), "f" (__q2y), "f" (__q2z) 
	);
	
	__q1x = q1.x; 
	__q1y = q1.y;
	__q2z = q2.z; 
	
	__asm__ volatile("\n"
					"fipr fv4, fv8 \n"
					"fmov fr11, fr1 \n"					
		: "+f" (__rx)
		: "f" (__q1w), "f" (__q1x), "f" (__q1y), "f" (__q1z),
		  "f" (__q2w), "f" (__q2x), "f" (__q2y), "f" (__q2z) 
	);
	
	__q1x = -q1.x; 
	__q1z = q1.z;
	__q2z = q2.z; 
	
	__asm__ volatile("\n"
					"fipr fv4, fv8 \n"
					"fmov fr11, fr2 \n"					
		: "+f" (__ry)
		: "f" (__q1w), "f" (__q1x), "f" (__q1y), "f" (__q1z),
		  "f" (__q2w), "f" (__q2x), "f" (__q2y), "f" (__q2z) 
	);
	
	__q1x = q1.x; 
	__q1y = -q1.y;
	__q2z = q2.z; 
	
	__asm__ volatile("\n"
					"fipr fv4, fv8 \n"
					"fmov fr11, fr3 \n"					
		: "+f" (__rz)
		: "f" (__q1w), "f" (__q1x), "f" (__q1y), "f" (__q1z),
		  "f" (__q2w), "f" (__q2x), "f" (__q2y), "f" (__q2z) 
	);
	
	out.w = __rw; 
    out.x = __rx; 
    out.y = __ry; 
    out.z = __rz; 
#else   
        __asm__ volatile("\n"
					//"fipr fv4, fv"
					"fmov fr8, fr12 \n" //
					"fmov fr9, fr13 \n" //
					"fmov fr10, fr14 \n"//
					"fmov fr11, fr15 \n"// reg w, x, y, z = q2 w, x, y, z
					"fipr fv4, fv12 \n"
					"fmov fr15, fr0 \n"
					
					"fneg fr5 \n"  // -(-q1.x)
					"fneg fr6 \n"  // -(-q1.y)
					"fmov fr9, fr12 \n" //reg w = q2.x
					"fmov fr8, fr13 \n" //reg x = q2.w
					"fmov fr11, fr14 \n" //reg y = q2.z
					"fmov fr10, fr15 \n" //reg z = q2.y
					"fipr fv4, fv12 \n"
					"fmov fr15, fr1 \n"
					  
					"fneg fr7 \n" //-(-q1.z)
					"fneg fr5 \n" //-(q1.x)
					"fmov fr10, fr12 \n" //reg w = q2.y
					"fmov fr8, fr13 \n" //reg x = q2.w
					"fmov fr9, fr14 \n" //reg y = q2.x
					"fmov fr11, fr15 \n" //reg z = q2.z
					"fmov fr11, fr15 \n"
					"fipr fv4, fv12 \n"
					"fmov fr15, fr2 \n"
					
					"fneg fr5 \n" // -(-q1.x)
					"fneg fr6 \n" // -(q1.y)		
					"fmov fr11, fr12 \n" //reg w = q2.x
					"fmov fr8, fr13 \n" //reg x = q2.w
					"fmov fr10, fr14 \n" //reg y = q2.z
					"fmov fr9, fr15 \n" //reg z = q2.y
					"fipr fv4, fv12 \n"
					"fmov fr15, fr3 \n"
					
		: "+f" (__rw), "+f" (__rx), "+f" (__ry), "+f" (__rz)
		: "f" (__q1w), "f" (__q1x), "f" (__q1y), "f" (__q1z),
		  "f" (__q2w), "f" (__q2x), "f" (__q2y), "f" (__q2z) 
	);
  
    out.w = __rw; 
    out.x = __rx; 
    out.y = __ry; 
    out.z = __rz; 
#endif
#endif
}

Quaternion Quaternion::Euler(float x, float y, float z)
{
	Quaternion qx, qy, qz; 
	qx = Quaternion::AxisAngle(1.0f, 0.0f, 0.0f, x); 
	qy = Quaternion::AxisAngle(0.0f, 1.0f, 0.0f, y); 
	qz = Quaternion::AxisAngle(0.0f, 0.0f, 1.0f, z); 
	return (qz*qy*qx); 	

#if 0
    float halfx = (x*(3.14/180.0f)) * 0.5f; 
    float halfy = (y*(3.14/180.0f)) * 0.5f;  
    float halfz = (z*(3.14/180.0f)) * 0.5f; 
    
    float cx = cosf(halfx);
    float sx = sinf(halfx);
    float cy = cosf(halfy); 
    float sy = sinf(halfy); 
    float cz = cosf(halfz); 
    float sz = sinf(halfz); 
    
    Quaternion q; 


    //q.x = sx*cy*cz - cx*sy*sz;
    //q.y = cx*sy*cz + sx*cy*sz; 
    //q.z = cx*cy*sz - sx*sy*cz; 
    //q.w = cx*cy*cz + sx*sy*sz;

//XYZ
    //q.w = cx*cy*cz - sx*sy*sz;
    //q.x = sy*cx*cz - sx*sz*cy; 
    //q.y = sx*sy*cz + sz*cx*cy; 
    //q.z = sx*cy*cz + sy*sz*cx;
//ZYX    
    q.w = sx*sy*sz + cx*cy*cz;
    q.x = sz*cx*cy + sx*sy*cz; 
    q.y = sx*sz*cy - sy*cx*cz; 
    q.z = sx*cy*cz - sy*sz*cx;
//YXZ
    //q.w = sx*sy*sz + cx*cy*cz;
    //q.x = sx*sz*cy + sy*cx*cz; 
    //q.y = sx*cy*cz - sy*sz*cx; 
    //q.z = sz*cx*cy - sx*sy*cz;
//YZX
    //q.w = cx*cy*cz - sx*sy*sz;
    //q.x = sx*sy*cz + sz*cx*cy; 
    //q.y = sx*cy*cz + sy*sz*cx; 
    //q.z = sy*cx*cz - sx*sz*cy;
//XZY
    //q.w = sx*sy*sz + cx*cy*cz;
    //q.x = sx*cy*cz - sy*sz*cx; 
    //q.y = sz*cx*cy - sx*sy*cz; 
    //q.z = sx*sz*cy + sy*cx*cz;
    
    //q.w = cz * cx * cy + sz * sx * sy;
    //q.x = cz * sx * cy - sz * cx * sy;
    //q.y = cz * cx * sy + sz * sx * cy;
    //q.z = sz * cx * cy - cz * sx * sy;
    
    //q.w = cz * cy * cx + sx * sy * sz;
    //q.x = sz * cy * cx - cx * sy * sz;
    //q.y = cz * sy * cx + sx * cy * sz;
    //q.z = cz * cy * sx - sx * sy * cz;
    
    return q;  
#endif                
}

Quaternion Quaternion::Normalize(Quaternion & q)
{
	float l = q.magnitude(); 
	return Quaternion(q.x/l, q.y/l, q.z/l, q.w); 
}

Quaternion Quaternion::Slerp(const Quaternion & q1, const Quaternion & q2, float l)
{
	float cosHalf = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z; 
	
	if (ABS(cosHalf) >= 1.0f)
		return q1; 
	 	
	float half = acosf(cosHalf); 
	float sinHalf = fsqrt(1.0f - cosHalf*cosHalf);
	
	if (ABS(sinHalf) < 0.001f)
	{
		return Quaternion(q1.x * 0.5f + q2.x * 0.5f,
						  q1.y * 0.5f + q2.y * 0.5f,
						  q1.z * 0.5f + q2.z * 0.5f,
						  q1.w * 0.5f + q2.w * 0.5f);
	} 
	
	float ratioA = fsin((1.0f - l) * half) / sinHalf; 
	float ratioB = fsin(l * half) / sinHalf; 
	
	return Quaternion(q1.x * ratioA + q2.x * ratioB,
					  q1.y * ratioA + q2.y * ratioB,
					  q1.z * ratioA + q2.z * ratioB,
					  q1.w * ratioA + q2.w * ratioB);
}

Quaternion Quaternion::LookRotation(const Vector & forward, const Vector & up = Vector::Up())
{
    Vector lookAt = forward;
    Vector rup = up;  
    Vector::OrthoNormalize(lookAt, rup);
    Vector right = Vector::Cross(rup, lookAt);  
    
    Quaternion ret; 
    ret.w = sqrt(1.0f + right.x + rup.y + lookAt.z) * 0.5f;
    float w4_recip = 1.0f / (4.0f * ret.w); 
    ret.x = (lookAt.y - rup.z) * w4_recip; 
    ret.y = (right.z - lookAt.x) * w4_recip; 
    ret.z = (rup.x - right.y) * w4_recip; 
    
    return ret; 
}

Quaternion Quaternion::Euler(const Vector & v)
{
    return Euler(v.x, v.y, v.z); 
}

/*Quaternion & Quaternion::operator /= (const Quaternion & q)
{

}
*/

/*
 *===============================================
 *Global functions / operators
 *===============================================
 */
Quaternion operator - (const Quaternion & q)
{
    return Quaternion(-q.x, -q.y, -q.z, -q.w); 
}

/*Quaternion operator - (const Quaternion & q1, const Quaternion & q2)
{
    return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, ); 
}

Quaternion operator + (const Quaternion & q1, const Quaternion & q2)
{

}
*/
Quaternion operator * (const Quaternion & q1, const Quaternion & q2)
{
    float ww = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z; 
    float xx = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y; 
    float yy = q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z;
    float zz = q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x; 
    	
    	//ww = -(q1.w*q2.w) + -(q1.x*q2.x) + -(q1.y*q2.y) -(q1.z*q2.z);
		
		//x =  (q1.x * q2.w) +  (q1.y * q2.z) + -(q1.z * q2.y) + (q1.w * q2.x);
		//y = -(q1.x * q2.z) +  (q1.y * q2.w) +  (q1.z * q2.x) + (q1.w * q2.y);
		//z =  (q1.x * q2.y) + -(q1.y * q2.x) +  (q1.z * q2.w) + (q1.w * q2.z);
		//w = -(q1.x * q2.x) + -(q1.y * q2.y) + -(q1.z * q2.z) + (q1.w * q2.w);

    //r->x = (q2.z * q1.y) - (q1.z * q2.y) + (q1.x * q2.w) + (q2.x * q1.w);
    //r->y = (q2.x * q1.z) - (q1.x * q2.z) + (q1.y * q2.w) + (q2.y * q1.w);
    //r->z = (q2.y * q1.x) - (q1.y * q2.x) + (q1.z * q2.w) + (q2.z * q1.w);
    //r->w = (q2.w * q1.w) - (q2.x * q1.x) - (q2.y * q1.y) - (q2.z * q1.z);
    
    return Quaternion(xx, yy, zz, ww); 
}
/*
Quaternion operator / (const Quaternion & q1, const Quaternion & q2)
{

}
*/

