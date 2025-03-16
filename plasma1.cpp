#include "sequences.h"
#include "ajHeaders.h" 
#include "geometry/ssphere_ply.h" 
#include "geometry/tsphere_ply.h" 
#include "input.h" 

pvr_poly_hdr_t plasma_header; 
Matrix transform; 

void render_plasma()
{

	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.2f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	vec3f_t tv __attribute__((aligned(32)));
	static float rv = 0; 
	static float ry = 0; 
	static float rz = 0; 
	static float freq = 5.3f;

	static float uOffset = 0;
	static float vOffset = 0; 
	float a = 0.8f; 
	float ay = 0.4f; 
	float az = 0.6f; 
	pvr_prim(&plasma_header, sizeof(plasma_header));
	pvr_dr_init(&dr_state);

	rv+=0.1f; 
	ry+=0.06f;
	rz+=0.03f;
	
	if (rv > 6.283f) rv = 0; 
	if (ry > 6.283f) ry = 0; 
	if (rz > 6.283f) rz = 0; 
	
	uOffset += 0.001f; 
	vOffset += 0.006f;
	
	if (uOffset >= 1.0f) uOffset = 0; 
	if (vOffset >= 1.0f) vOffset = 0; 
	
	for(int i = 0; i < SSPHERE_FACE_COUNT; ++i)
	{

	tv.x = ssphere_verts[ssphere_faces[i].x].x + a * fcos((ssphere_verts[ssphere_faces[i].x].z)*freq - rv); 
	tv.y = ssphere_verts[ssphere_faces[i].x].y + ay * fsin((ssphere_verts[ssphere_faces[i].x].x)*freq - ry); 
	tv.z = ssphere_verts[ssphere_faces[i].x].z + az * fsin((ssphere_verts[ssphere_faces[i].x].y)*freq - rz); 

	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = ssphere_uvs[ssphere_faces[i].x].u + uOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].x].v + vOffset;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	tv.x = ssphere_verts[ssphere_faces[i].y].x + a * fcos((ssphere_verts[ssphere_faces[i].y].z)*freq - rv); 
	tv.y = ssphere_verts[ssphere_faces[i].y].y + ay * fsin((ssphere_verts[ssphere_faces[i].y].x)*freq - ry); 
	tv.z = ssphere_verts[ssphere_faces[i].y].z + az * fsin((ssphere_verts[ssphere_faces[i].y].y)*freq - rz); 

	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 

		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = ssphere_uvs[ssphere_faces[i].y].u + uOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].y].v + vOffset;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	tv.x = ssphere_verts[ssphere_faces[i].z].x + a * fcos((ssphere_verts[ssphere_faces[i].z].z)*freq - rv); 
	tv.y = ssphere_verts[ssphere_faces[i].z].y + ay * fsin((ssphere_verts[ssphere_faces[i].z].x)*freq - ry); 
	tv.z = ssphere_verts[ssphere_faces[i].z].z + az * fsin((ssphere_verts[ssphere_faces[i].z].y)*freq - rz); 
		
	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 

		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = ssphere_uvs[ssphere_faces[i].z].u + uOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].z].v + vOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}

	pvr_dr_finish();
}

void PlasmaInit()
{
	kos_texture_t* platex = load_png_texture("/rd/wave1.png");
	CompileHeaderWithTexture(plasma_header, TRTYPE, *platex);
	
	Matrix plaT, plaS, plaR; 
	
	plaT.translate(0.0f, 0.0f, 8.0f);
    plaR.rotateY(45*PI/180.0f);
    plaS.scale(3.6f, 3.6f, 3.6f);
    
    
    mat_identity();
   	mat_apply(&proj.elem);
   	mat_apply(&plaT.elem); 
   	mat_apply(&plaR.elem);
   	mat_apply(&plaS.elem);
   	mat_store(&transform.elem);
}

void PlasmaRun()
{
       	static float r; 
       	static int i = 0; 
       	
       	if (gInput.buttonDown(BN_CROSS))
       		i ^= 1; 
       	
       	r +=0.01f;
       	pvr_wait_ready();
       	pvr_scene_begin();
       	pvr_list_begin(PVR_LIST_TR_POLY);
       		
       		mat_identity();
       		mat_apply(&transform.elem);
       		     	
       		render_plasma();
       	pvr_list_finish();
		pvr_scene_finish();
}
