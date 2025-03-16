#include "sequences.h"
#include "ajHeaders.h" 
#include "geometry/negsphere_ply.h" 
#include "geometry/plane_ply.h" 
#include "input.h" 
#include <cmath>

pvr_poly_hdr_t negsphere_header; 
pvr_poly_hdr_t negsphere_shadow_header; 
pvr_poly_hdr_t negsphere_fg_header; 

Quaternion negRot; 

void render_negsphere(float mmlerp)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 1.0f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	
	float shininess = lerpf(12, 2, mmlerp);
	float intensity = 1.32f; 
	static Vector normal;
	static Vector eye(0, 0, -1);
	Vector l = Vector::Normalize(Vector(0.0f, 0.0f, -1.0f));
	Vector half = Vector::Normalize(l + eye);
	Vector cullDir = Quaternion::Conjugate(negRot) * -Vector::Right();
	static float res, real;
	
	pvr_prim(&negsphere_header, sizeof(negsphere_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < NEGSPHERE_FACE_COUNT; ++i)
	{
		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		normal.x = negsphere_norms[negsphere_faces[i].x].x; 
		normal.y = negsphere_norms[negsphere_faces[i].x].y; 
		normal.z = negsphere_norms[negsphere_faces[i].x].z;
		
		if (Vector::Dot(normal, cullDir) < -0.356f) continue; 
		
		normal = negRot * normal; 
		
	mat_trans_single3_nomod(negsphere_verts[negsphere_faces[i].x].x,
							negsphere_verts[negsphere_faces[i].x].y,
							negsphere_verts[negsphere_faces[i].x].z,
							v.x, v.y, v.z);
		
		res = MAX(0, Vector::Dot(normal, half));
		res = fastPow(res, shininess); 
		res *= intensity;
		real = lerpf(res, MAX(1.0f, res), mmlerp);
		r = b = g = real; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = negsphere_uvs[negsphere_faces[i].x].u; 
		tvert->v = negsphere_uvs[negsphere_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(negsphere_verts[negsphere_faces[i].y].x,
							negsphere_verts[negsphere_faces[i].y].y,
							negsphere_verts[negsphere_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		
		normal.x = negsphere_norms[negsphere_faces[i].y].x; 
		normal.y = negsphere_norms[negsphere_faces[i].y].y; 
		normal.z = negsphere_norms[negsphere_faces[i].y].z;
		
		normal = negRot * normal; 
		
		res = MAX(0, Vector::Dot(normal, half));
		res = fastPow(res, shininess); 
		res *= intensity;
		real = lerpf(res, MAX(1.0f, res), mmlerp);
		r = b = g = real;
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = negsphere_uvs[negsphere_faces[i].y].u; 
		tvert->v = negsphere_uvs[negsphere_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(negsphere_verts[negsphere_faces[i].z].x,
							negsphere_verts[negsphere_faces[i].z].y,
							negsphere_verts[negsphere_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		
		normal.x = negsphere_norms[negsphere_faces[i].z].x; 
		normal.y = negsphere_norms[negsphere_faces[i].z].y; 
		normal.z = negsphere_norms[negsphere_faces[i].z].z;
		
		normal = negRot * normal; 
		
		res = MAX(0, Vector::Dot(normal, half));
		res = fastPow(res, shininess); 
		res *= intensity;
		real = lerpf(res, MAX(1.0f, res), mmlerp);
		r = b = g = real;
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = negsphere_uvs[negsphere_faces[i].z].u; 
		tvert->v = negsphere_uvs[negsphere_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void render_shadow_negsphere(float alpha, float rgb)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;

	float r = rgb, g = rgb, b = rgb;
	vec3f_t v __attribute__((aligned(32)));

	pvr_prim(&negsphere_shadow_header, sizeof(negsphere_shadow_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < NEGSPHERE_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(negsphere_verts[negsphere_faces[i].x].x,
							negsphere_verts[negsphere_faces[i].x].y,
							negsphere_verts[negsphere_faces[i].x].z,
							v.x, v.y, v.z);
							
		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = negsphere_uvs[negsphere_faces[i].x].u; 
		tvert->v = negsphere_uvs[negsphere_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(negsphere_verts[negsphere_faces[i].y].x,
							negsphere_verts[negsphere_faces[i].y].y,
							negsphere_verts[negsphere_faces[i].y].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = negsphere_uvs[negsphere_faces[i].y].u; 
		tvert->v = negsphere_uvs[negsphere_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(negsphere_verts[negsphere_faces[i].z].x,
							negsphere_verts[negsphere_faces[i].z].y,
							negsphere_verts[negsphere_faces[i].z].z,
							v.x, v.y, v.z);
						
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = negsphere_uvs[negsphere_faces[i].z].u; 
		tvert->v = negsphere_uvs[negsphere_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void render_fg_negsphere(float alpha)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;

	float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));

	pvr_prim(&negsphere_fg_header, sizeof(negsphere_fg_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < PLANE_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(plane_verts[plane_faces[i].x].x,
							plane_verts[plane_faces[i].x].y,
							plane_verts[plane_faces[i].x].z,
							v.x, v.y, v.z);
							
		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = plane_uvs[plane_faces[i].x].u; 
		tvert->v = plane_uvs[plane_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(plane_verts[plane_faces[i].y].x,
							plane_verts[plane_faces[i].y].y,
							plane_verts[plane_faces[i].y].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = plane_uvs[plane_faces[i].y].u; 
		tvert->v = plane_uvs[plane_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(plane_verts[plane_faces[i].z].x,
							plane_verts[plane_faces[i].z].y,
							plane_verts[plane_faces[i].z].z,
							v.x, v.y, v.z);
						
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = plane_uvs[plane_faces[i].z].u; 
		tvert->v = plane_uvs[plane_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void NegEffectInit()
{
	kos_texture_t* fgtex = load_png_texture("/rd/negfg.png");
	
	CompileHeader(negsphere_header, OPTYPE);
	CompileHeader(negsphere_shadow_header, TRTYPE);
	CompileHeaderWithTexture(negsphere_fg_header, TRTYPE, *fgtex);
}

void NegEffectRun()
{
	static uint64_t lerpTimer = timer_ms_gettime64(); 
	static int timerContext = 0; 
	static float timeList[]
	{
		85, 
		53
	};
	
	static Matrix negT, negS, negR;//x, negRy, negRz; 
	static float rx, ry, rz; 
	static float fpos = 0; 
	static float spos = 0;
	
	static float fsrgb = 0; 
	static float fscale = 0; 
	static float sscale = 0; 

	static float palpha = 0; 
	
	static float lerpValue = 0.0f; 
	lerpValue += 0.03f; 
	if (lerpValue > 1.0f) lerpValue = 1.0f; 
	
	fscale = lerpf(4.2f, 2.2f, lerpValue);
	sscale = lerpf(12.2f, 3.6f, lerpValue);
	
	fsrgb = lerpf(0.0f, 1.0f, lerpValue);
	palpha = lerpf(1.0f, 0.1f, lerpValue);
	if (gInput.buttonDown(BN_CROSS)) lerpValue = 0;
	
	if (lerpTimer < timer_ms_gettime64())
	{
		lerpValue = 0; 
		lerpTimer += timeList[timerContext]*10; 
		timerContext ^= 1; 
	}
	
	rx += 0.8f; 
	ry += 3.3f; 
	rz += 1.5f; 

	negRot = Quaternion::Euler(rx, ry, rz);
	
	negR.quaternionToMatrix(negRot);
	
	pvr_wait_ready();
   	pvr_scene_begin();
   	pvr_list_begin(PVR_LIST_OP_POLY);
	
		negT.translate(0, 0, 10.0f);	
		negS.scale(1.0f, 1.0f, 1.0f);
		
	   	mat_identity();
		mat_apply(&proj.elem);
    	mat_apply(&negT.elem); 
    	mat_apply(&negR.elem);
    	mat_apply(&negS.elem);
    	
    	render_negsphere(lerpValue);
    	
   	pvr_list_finish();
   	pvr_list_begin(PVR_LIST_TR_POLY);
   	    
   	    negT.translate(fpos, fpos, 15.0f);	
		negS.scale(fscale, fscale, 0.0f);
		
	   	mat_identity();
		mat_apply(&proj.elem); 
    	mat_apply(&negT.elem); 
    	mat_apply(&negS.elem);
    	mat_apply(&negR.elem);
    	
    	render_shadow_negsphere(0.3f, 1.0f);
    	
    	negT.translate(spos, spos, 20.0f);	
		negS.scale(sscale, sscale, 0.0f);
		
	   	mat_identity();
		mat_apply(&proj.elem); 
    	mat_apply(&negT.elem); 
    	mat_apply(&negS.elem);
    	mat_apply(&negR.elem);
    	
    	render_shadow_negsphere(0.08f, fsrgb);
    	
    	negT.translate(0, 0, 21.0f);	
		negS.scale(13.0f, 13.0f, 0.0f);
		
	   	mat_identity();
		mat_apply(&proj.elem); 
    	mat_apply(&negT.elem); 
    	mat_apply(&negS.elem);
    	
    	render_fg_negsphere(palpha);
   	pvr_list_finish();
	pvr_scene_finish();     	
}

