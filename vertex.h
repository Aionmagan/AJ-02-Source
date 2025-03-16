#ifndef __VERTEX_H__
#define __VERTEX_H__

typedef struct __attribute__((aligned(32)))
{
    float x, y, z, w;
}VECTOR;

typedef struct
{
    int x, y, z;
}FACES;

#endif//__VERTEX_H__
