#ifndef __PLY_H__
#define __PLY_H__

#include <vector>
#include <string>

#include <kos.h>
#include "common.h" 
#include "pngloader.h"

//enum PlyHeaderType
//{
//	OPTYPE,
//	TRTYPE
//};

struct Vertex
{
	float x, y, z; 
}AJ_ALIGNED;

struct UV
{
	float u, v; 
}AJ_ALIGNED;

struct Color
{
	float r, g, b, a; 
}AJ_ALIGNED;

struct Face
{
	unsigned int x, y, z; 
}AJ_ALIGNED;
#if 0
class AJ_ALIGNED PlyMesh
{
public:
	std::vector<Vertex>  verts; 
	std::vector<UV> 	 uvs; 
	std::vector<Vertex>  norms;
	std::vector<Color>   colors; 
	std::vector<Face>    faces;
	int               	 face_count; 

	pvr_poly_hdr_t ply_header;
public:
	PlyMesh(std::string filename, PlyHeaderType type = OPTYPE);
	PlyMesh(); //for data oriented 
   ~PlyMesh(); 	
   	
   	void setPlyType(PlyHeaderType type);
   	void setPlyTypeWithTexture(PlyHeaderType type, kos_texture_t & kt);
   	void freeMemory(); 
   	bool load(std::string filename); 
};
#endif 
#endif/*__PLY_H__*/
