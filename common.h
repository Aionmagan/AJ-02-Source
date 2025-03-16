#ifndef __COMMON_H__
#define __COMMON_H__

#include <kos.h>
#include "pngloader.h"

#define PI     3.1415926535897932384626433832795f
#define TWOPI  6.283185307179586476925286766559f
#define HALFPI 1.5707963267948966192313216916398f

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define DEG2RAD(x) (x*(PI/180.0))
#define RAD2DEG(x) (x*(180.0/PI)) 

#define ABS(a) (a>0?a:(0-a))

#define AJ_ALIGNED __attribute__((__aligned__(32)))

#define ARRAYSIZE(x)(sizeof(x)/sizeof(x[0]))

enum PlyHeaderType
{
	OPTYPE,
	TRTYPE
};

float lerpf(float a, float b, float d);
double fastPow(double base, int exponent);

void CompileHeader(pvr_poly_hdr_t& ply_header, PlyHeaderType type, int flag = 0);
void CompileHeaderWithTexture(pvr_poly_hdr_t& ply_header, PlyHeaderType type, kos_texture_t & kt, int flag = 0);

#endif/*__COMMON_H__*/
