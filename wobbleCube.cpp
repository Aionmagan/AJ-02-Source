#include "sequences.h"
#include "ajHeaders.h" 
#include "geometry/crystal_ply.h"
#include "geometry/ssphere_ply.h"
#include "geometry/cubelevel_ply.h"

#include "input.h" 

#include <cmath>

pvr_poly_hdr_t wobble_cube_header; 
pvr_poly_hdr_t wobble_level_header; 
pvr_poly_hdr_t wobble_cryst_header; 
Camera wobCam; 

Quaternion wobLightDir; 
Quaternion wobRot; 
Quaternion culldir; 

uint64_t wobStartTimer; 
uint64_t wobStopTimer; 

void render_wobble_cube(bool shift)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 1.0f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	float rr=0.0f, gg = 0.4f, bb = 0.25f; 
	vec3f_t v __attribute__((aligned(32)));
	vec3f_t tv __attribute__((aligned(32)));
	static float rv = 0.0f; 
	static float ry = 4.7f; 
	static float rz = 4.7f; 
	static float ur = 0; 
	static float vr = 0;
	static float freq = 3;

	pvr_prim(&wobble_cube_header, sizeof(wobble_cube_header));
	pvr_dr_init(&dr_state);
	float uOffset = 0; float vOffset = 0;
	
	float intensity = 1.0f; 
	Vector normal;

	static Vector light = Vector::Normalize(Vector(-1.0f, 0.5f, -1.0f));
	Vector l = (wobLightDir) * light;
	Vector eye = Quaternion::Conjugate(culldir) * -Vector::Right();

	static float sxr = 0.0f; 
	static float syr = 4.7f; 
	static float szr = 4.7f; 
	static float lerpValue = 1; 
	
	ur += 0.001f;
	vr += 0.003f;
	
	if (shift)
	{	
		rv+=0.01f; 
		ry+=0.2f;
		rz+=0.07f;
		lerpValue = 0;
	}
	else
	{
		rv = lerpf(rv, sxr, lerpValue);
		ry = lerpf(ry, syr, lerpValue);
		rz = lerpf(rz, szr, lerpValue);
		lerpValue += 0.002f;
		if (lerpValue > 1.0f) lerpValue = 1.0f;
	}
	
	if (rv > 6.283f) rv = 0; 
	if (ry > 6.283f) ry = 0; 
	if (rz > 6.283f) rz = 0; 
	
	for(int i = 0; i < SSPHERE_FACE_COUNT; ++i)
	{
		normal.x = ssphere_norms[ssphere_faces[i].x].x;
		normal.y = ssphere_norms[ssphere_faces[i].x].y;
		normal.z = ssphere_norms[ssphere_faces[i].x].z;

		if (Vector::Dot(normal, eye) < -0.356f) continue; 
	
	tv.x = ssphere_verts[ssphere_faces[i].x].x + ssphere_norms[ssphere_faces[i].x].x * (fcos((ssphere_norms[ssphere_faces[i].x].x)*freq - rv)*0.5f); 
	tv.y = ssphere_verts[ssphere_faces[i].x].y + ssphere_norms[ssphere_faces[i].x].y * (fsin((ssphere_norms[ssphere_faces[i].x].y)*freq - ry)*0.5f); 
	tv.z = ssphere_verts[ssphere_faces[i].x].z + ssphere_norms[ssphere_faces[i].x].z * (fsin((ssphere_norms[ssphere_faces[i].x].z)*freq - rz)*0.5f); 
	 
	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 

		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr + intensity; 
		b = bb + intensity; 
		g = gg + intensity; 

		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  
		uOffset = fcos(ur);
		vOffset = fsin(vr); 
		tvert->u = ssphere_uvs[ssphere_faces[i].x].u + uOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].x].v + vOffset;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	tv.x = ssphere_verts[ssphere_faces[i].y].x + ssphere_norms[ssphere_faces[i].y].x * (fcos((ssphere_norms[ssphere_faces[i].y].x)*freq - rv)*0.5f); 
	tv.y = ssphere_verts[ssphere_faces[i].y].y + ssphere_norms[ssphere_faces[i].y].y * (fsin((ssphere_norms[ssphere_faces[i].y].y)*freq - ry)*0.5f); 
	tv.z = ssphere_verts[ssphere_faces[i].y].z + ssphere_norms[ssphere_faces[i].y].z * (fsin((ssphere_norms[ssphere_faces[i].y].z)*freq - rz)*0.5f); 

	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
	
		normal.x = ssphere_norms[ssphere_faces[i].y].x;
		normal.y = ssphere_norms[ssphere_faces[i].y].y;
		normal.z = ssphere_norms[ssphere_faces[i].y].z;
		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr + intensity; 
		b = bb + intensity; 
		g = gg + intensity; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = ssphere_uvs[ssphere_faces[i].y].u + uOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].y].v + vOffset;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	tv.x = ssphere_verts[ssphere_faces[i].z].x + ssphere_norms[ssphere_faces[i].z].x * (fcos((ssphere_norms[ssphere_faces[i].z].x)*freq - rv)*0.5f); 
	tv.y = ssphere_verts[ssphere_faces[i].z].y + ssphere_norms[ssphere_faces[i].z].y * (fsin((ssphere_norms[ssphere_faces[i].z].y)*freq - ry)*0.5f); 
	tv.z = ssphere_verts[ssphere_faces[i].z].z + ssphere_norms[ssphere_faces[i].z].z * (fsin((ssphere_norms[ssphere_faces[i].z].z)*freq - rz)*0.5f); 
		
	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 

		normal.x = ssphere_norms[ssphere_faces[i].z].x;
		normal.y = ssphere_norms[ssphere_faces[i].z].y;
		normal.z = ssphere_norms[ssphere_faces[i].z].z;
		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr + intensity; 
		b = bb + intensity; 
		g = gg + intensity; 
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

void render_wobble_level()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 1.0f; 
	float r = 0.2f, g = 0.35f, b = 0.2f;
	vec3f_t v __attribute__((aligned(32)));

	 float w = 0;
	pvr_prim(&wobble_level_header, sizeof(wobble_level_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < CUBELEVEL_FACE_COUNT; ++i)
	{
	v.x = cubelevel_verts[cubelevel_faces[i].x].x;
	v.y = cubelevel_verts[cubelevel_faces[i].x].y;
	v.z = cubelevel_verts[cubelevel_faces[i].x].z;
	
	mat_trans_single3_nodivw(v.x,v.y,v.z,w); 

if (v.z < 4.0f) continue;

	mat_trans_single3_nomod(cubelevel_verts[cubelevel_faces[i].x].x,
							cubelevel_verts[cubelevel_faces[i].x].y,
							cubelevel_verts[cubelevel_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
	
		tvert->argb = PVR_PACK_COLOR(alpha, 
						  cubelevel_colors[cubelevel_faces[i].x].r*r, 
						  cubelevel_colors[cubelevel_faces[i].x].g*g,
						  cubelevel_colors[cubelevel_faces[i].x].b*b);						  
		tvert->oargb = 0;
						  	
		tvert->u = cubelevel_uvs[cubelevel_faces[i].x].u; 
		tvert->v = cubelevel_uvs[cubelevel_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(cubelevel_verts[cubelevel_faces[i].y].x,
							cubelevel_verts[cubelevel_faces[i].y].y,
							cubelevel_verts[cubelevel_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 

		tvert->argb = PVR_PACK_COLOR(alpha, 
						  cubelevel_colors[cubelevel_faces[i].y].r*r, 
						  cubelevel_colors[cubelevel_faces[i].y].g*g,
						  cubelevel_colors[cubelevel_faces[i].y].b*b);
						  
		tvert->oargb = 0;								

		tvert->u = cubelevel_uvs[cubelevel_faces[i].y].u; 
		tvert->v = cubelevel_uvs[cubelevel_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(cubelevel_verts[cubelevel_faces[i].z].x,
							cubelevel_verts[cubelevel_faces[i].z].y,
							cubelevel_verts[cubelevel_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 

		tvert->argb = PVR_PACK_COLOR(alpha, 
						  cubelevel_colors[cubelevel_faces[i].z].r*r, 
						  cubelevel_colors[cubelevel_faces[i].z].g*g,
						  cubelevel_colors[cubelevel_faces[i].z].b*b);
		tvert->oargb = 0;								

		tvert->u = cubelevel_uvs[cubelevel_faces[i].z].u; 
		tvert->v = cubelevel_uvs[cubelevel_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void render_wobble_crystal()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.3f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	float rr = 0.9f, gg = 0.1f, bb = 0.1f, aa = 0.8f;
	vec3f_t v __attribute__((aligned(32)));
	
	static Vector light = Vector::Normalize(Vector(-1.0f, -0.5, -1.0f));
	static float x, y; 
	static Vector l;
	x += 1.8f; 
	y += 1.5f; 
	l = Quaternion::Euler(0, x, y) * Quaternion::Conjugate(culldir) * light; 

	static Vector normal; 
	static float intensity; 
	
	pvr_prim(&wobble_cryst_header, sizeof(wobble_cryst_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < CRYSTAL_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(crystal_verts[crystal_faces[i].x].x,
							crystal_verts[crystal_faces[i].x].y,
							crystal_verts[crystal_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		normal.x = crystal_norms[crystal_faces[i].x].x; 
		normal.y = crystal_norms[crystal_faces[i].x].y; 
		normal.z = crystal_norms[crystal_faces[i].x].z; 

		intensity = MAX(ABS(Vector::Dot(normal, l)), 0);
		alpha = MIN(0.3f, aa * intensity);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = crystal_uvs[crystal_faces[i].x].u; 
		tvert->v = crystal_uvs[crystal_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(crystal_verts[crystal_faces[i].y].x,
							crystal_verts[crystal_faces[i].y].y,
							crystal_verts[crystal_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
			
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);

		tvert->oargb = 0;								

		tvert->u = crystal_uvs[crystal_faces[i].y].u; 
		tvert->v = crystal_uvs[crystal_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(crystal_verts[crystal_faces[i].z].x,
							crystal_verts[crystal_faces[i].z].y,
							crystal_verts[crystal_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = crystal_uvs[crystal_faces[i].z].u; 
		tvert->v = crystal_uvs[crystal_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void WobbleCubeInit()
{
	kos_texture_t* wctex = load_png_texture("/rd/wave1.png");

	CompileHeaderWithTexture(wobble_cube_header, OPTYPE, *wctex);
	CompileHeader(wobble_level_header, OPTYPE);
	CompileHeader(wobble_cryst_header, TRTYPE); 

	wobStartTimer = 9403*10+timer_ms_gettime64(); 
	wobStopTimer = 10070*10+timer_ms_gettime64(); 
	//start 9660
	//stop 10070
}

void WobbleCubeRun()
{
	static Matrix wobT, wobR, wobS;
	static Matrix wobRl, wobTl;
	
	static float rx = 0, ry = 0, rz = 0;
	static float sf = 0, cf = 0; 
	static bool shiftShape = false; 
	static Vector lookTarget = Vector::Zero(); 
	static Vector crystPos; 
	static Vector crystOffset = Vector::Normalize(Vector(-1.0f, 0.5f, -1.0f));
	
	sf += 0.003f; 
	cf += 0.007f; 
	
	rx += 1.9f*fsin(sf); 
	ry += 0.78f*fsin(cf); 
	rz += 2.23f*fcos(cf);

	wobRot = Quaternion::Euler(rx, ry, rz);
	wobR.quaternionToMatrix(wobRot);
	
	wobLightDir = wobRot; 
	wobRot = Quaternion::Euler(0, rz, 40.0f * fsin(rx*0.01f));

	wobCam.position = wobRot * Vector::Forward() * 8; 
	wobCam.lookAt(lookTarget);
	wobCam.applyView();
	
	crystPos = wobLightDir * wobLightDir * crystOffset * 3.0f;
	culldir = wobCam.rotation * Quaternion::Euler(rx, ry, rz); 

	if (wobStartTimer < timer_ms_gettime64()) shiftShape = true;
	if (wobStopTimer < timer_ms_gettime64()) shiftShape = false;
	
	pvr_wait_ready();
   	pvr_scene_begin();
   	pvr_list_begin(PVR_LIST_OP_POLY);

	   	mat_identity();
		mat_apply(&proj.elem); 
		mat_apply(&Camera::view.elem); 
    	mat_apply(&wobT.elem); 
    	mat_apply(&wobS.elem);
    	mat_apply(&wobRl.elem);
    	
    	render_wobble_level();

	   	mat_identity();
		mat_apply(&proj.elem); 
		mat_apply(&Camera::view.elem);
    	mat_apply(&wobT.elem); 
    	mat_apply(&wobS.elem);
    	mat_apply(&wobR.elem);
    	
    	render_wobble_cube(shiftShape);

   	pvr_list_finish();
   	pvr_list_begin(PVR_LIST_TR_POLY);
   		
   		wobTl.translate(crystPos.x, crystPos.y, crystPos.z);
   		
   		mat_identity();
		mat_apply(&proj.elem); 
		mat_apply(&Camera::view.elem);
    	mat_apply(&wobTl.elem); 
    	mat_apply(&wobS.elem);
    	mat_apply(&wobR.elem);
    	
    	render_wobble_crystal();
   	pvr_list_finish();
	pvr_scene_finish(); 
}
