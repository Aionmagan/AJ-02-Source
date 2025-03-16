/*Generated using plyProgram by Aionmagan*/
#ifndef __PLANE_H__
#define __PLANE_H__

#include "../ajHeaders.h" 

#define PLANE_VERT_COUNT 4
#define PLANE_FACE_COUNT 2

/*Data generated*/
/*plane_verts[]*/
/*plane_norms[]*/
/*plane_uvs[]*/
/*plane_faces[]*/

static const Vertex plane_verts[] = {
   {.x=1.000000, .y=-1.000000, .z=0.000000},
   {.x=-1.000000, .y=1.000000, .z=0.000000},
   {.x=-1.000000, .y=-1.000000, .z=0.000000},
   {.x=1.000000, .y=1.000000, .z=0.000000},
};



static const Vertex plane_norms[] = {
   {.x=-0.000000, .y=0.000000, .z=1.000000},
   {.x=-0.000000, .y=0.000000, .z=1.000000},
   {.x=-0.000000, .y=0.000000, .z=1.000000},
   {.x=-0.000000, .y=0.000000, .z=1.000000},
};



static const UV plane_uvs[] = {
   {.u=1.000000, .v=0.000000},
   {.u=0.000000, .v=1.000000},
   {.u=0.000000, .v=0.000000},
   {.u=1.000000, .v=1.000000},
};



static const Face plane_faces[] = {
   {.x=0, .y=1, .z=2},
   {.x=0, .y=3, .z=1},
};


#endif /*__PLANE_H__*/
