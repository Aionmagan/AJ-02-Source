#include "sequences.h"
#include "ajHeaders.h" 
#include "cmath" 
#include "geometry/ftorus_ply.h"
#include "geometry/intro_ply.h"
#include "geometry/dd_ply.h"
#include "geometry/plane_ply.h"

Camera introCam; 

pvr_poly_hdr_t name_header;
pvr_poly_hdr_t sname_header;
pvr_poly_hdr_t ftorus_header;
pvr_poly_hdr_t ftorus_shadow_header;
pvr_poly_hdr_t ibg_header; 

static Matrix introS, introR, introT; 

void render_ibg()
{

}

void render_ftorus()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.7f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	float uOffset = 0, vOffset = 0;
	
	pvr_prim(&ftorus_header, sizeof(ftorus_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < FTORUS_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(ftorus_verts[ftorus_faces[i].x].x,
							ftorus_verts[ftorus_faces[i].x].y,
							ftorus_verts[ftorus_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = ftorus_uvs[ftorus_faces[i].x].u + uOffset; 
		tvert->v = ftorus_uvs[ftorus_faces[i].x].v + vOffset;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(ftorus_verts[ftorus_faces[i].y].x,
							ftorus_verts[ftorus_faces[i].y].y,
							ftorus_verts[ftorus_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = ftorus_uvs[ftorus_faces[i].y].u + uOffset; 
		tvert->v = ftorus_uvs[ftorus_faces[i].y].v + vOffset;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(ftorus_verts[ftorus_faces[i].z].x,
							ftorus_verts[ftorus_faces[i].z].y,
							ftorus_verts[ftorus_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = ftorus_uvs[ftorus_faces[i].z].u + uOffset; 
		tvert->v = ftorus_uvs[ftorus_faces[i].z].v + vOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}


void render_ftorus_shadow()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.3f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	float uOffset = 0, vOffset = 0;
	
	pvr_prim(&ftorus_shadow_header, sizeof(ftorus_shadow_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < FTORUS_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(ftorus_verts[ftorus_faces[i].x].x,
							ftorus_verts[ftorus_faces[i].x].y,
							ftorus_verts[ftorus_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = ftorus_uvs[ftorus_faces[i].x].u + uOffset; 
		tvert->v = ftorus_uvs[ftorus_faces[i].x].v + vOffset;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(ftorus_verts[ftorus_faces[i].y].x,
							ftorus_verts[ftorus_faces[i].y].y,
							ftorus_verts[ftorus_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = ftorus_uvs[ftorus_faces[i].y].u + uOffset; 
		tvert->v = ftorus_uvs[ftorus_faces[i].y].v + vOffset;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(ftorus_verts[ftorus_faces[i].z].x,
							ftorus_verts[ftorus_faces[i].z].y,
							ftorus_verts[ftorus_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = ftorus_uvs[ftorus_faces[i].z].u + uOffset; 
		tvert->v = ftorus_uvs[ftorus_faces[i].z].v + vOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void render_name()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 1.0f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	static float rr = 0.8f, gg=0.8f, bb=0.8f; 
	vec3f_t v __attribute__((aligned(32)));
	static float uOffset = 0, vOffset = 0;

	float intensity; 
	Vector normal;
	static Vector l = Vector::Normalize(Vector(-1.4f, 1.0f, -1.0f));

	pvr_prim(&name_header, sizeof(name_header));
	pvr_dr_init(&dr_state);
	
	static float xr = 0; 
	static float yr = 0; 
	static float zr = 0;
	
	vOffset -= 0.0001f;
	uOffset -= 0.00075f;
	if (vOffset < -1.0f) vOffset = 0; 
	if (uOffset < -1.0f) uOffset = 0; 

	if (xr > 6.283f) xr = 0; 
	if (yr > 6.283f) yr = 0; 
	if (zr > 6.283f) zr = 0; 

	for(int i = 0; i < INTRO_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(intro_verts[intro_faces[i].x].x,
							intro_verts[intro_faces[i].x].y,
							intro_verts[intro_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		normal.x = intro_norms[intro_faces[i].x].x; 
		normal.y = intro_norms[intro_faces[i].x].y; 
		normal.z = intro_norms[intro_faces[i].x].z; 
		
		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = intro_uvs[intro_faces[i].x].u + uOffset; 
		tvert->v = intro_uvs[intro_faces[i].x].v + vOffset;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(intro_verts[intro_faces[i].y].x,
							intro_verts[intro_faces[i].y].y,
							intro_verts[intro_faces[i].y].z,
							v.x, v.y, v.z);

		normal.x = intro_norms[intro_faces[i].y].x; 
		normal.y = intro_norms[intro_faces[i].y].y; 
		normal.z = intro_norms[intro_faces[i].y].z; 										
											
		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = intro_uvs[intro_faces[i].y].u + uOffset; 
		tvert->v = intro_uvs[intro_faces[i].y].v + vOffset;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(intro_verts[intro_faces[i].z].x,
							intro_verts[intro_faces[i].z].y,
							intro_verts[intro_faces[i].z].z,
							v.x, v.y, v.z);								
		
		normal.x = intro_norms[intro_faces[i].z].x; 
		normal.y = intro_norms[intro_faces[i].z].y; 
		normal.z = intro_norms[intro_faces[i].z].z; 										
		
		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 			
											
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = intro_uvs[intro_faces[i].z].u + uOffset; 
		tvert->v = intro_uvs[intro_faces[i].z].v + vOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}


void render_name_shadow()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.3f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	static float uOffset = 0, vOffset = 0;

	pvr_prim(&sname_header, sizeof(sname_header));
	pvr_dr_init(&dr_state);


	for(int i = 0; i < DD_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(dd_verts[dd_faces[i].x].x,
							dd_verts[dd_faces[i].x].y,
							dd_verts[dd_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = dd_uvs[dd_faces[i].x].u + uOffset; 
		tvert->v = dd_uvs[dd_faces[i].x].v + vOffset;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(dd_verts[dd_faces[i].y].x,
							dd_verts[dd_faces[i].y].y,
							dd_verts[dd_faces[i].y].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = dd_uvs[dd_faces[i].y].u + uOffset; 
		tvert->v = dd_uvs[dd_faces[i].y].v + vOffset;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(dd_verts[dd_faces[i].z].x,
							dd_verts[dd_faces[i].z].y,
							dd_verts[dd_faces[i].z].z,
							v.x, v.y, v.z);							

		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = dd_uvs[dd_faces[i].z].u + uOffset; 
		tvert->v = dd_uvs[dd_faces[i].z].v + vOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void render_name_ibg(float tuOffset, float vOffset)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 1.0f; 
	float r = 0.8f, g = 0.8f, b = 0.8f;
	vec3f_t v __attribute__((aligned(32)));

	static float uOffset = 0;
	uOffset += 0.001f; 
	if (uOffset >= 1.0f) uOffset = 0;
	
	pvr_prim(&ibg_header, sizeof(ibg_header));
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
						  	
		tvert->u = plane_uvs[plane_faces[i].x].u + uOffset; 
		tvert->v = plane_uvs[plane_faces[i].x].v + vOffset;
					
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

		tvert->u = plane_uvs[plane_faces[i].y].u + uOffset; 
		tvert->v = plane_uvs[plane_faces[i].y].v + vOffset;
	
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

		tvert->u = plane_uvs[plane_faces[i].z].u + uOffset; 
		tvert->v = plane_uvs[plane_faces[i].z].v + vOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void IntroInit()
{

	kos_texture_t* nametex = load_png_texture("/rd/intro.png");

	CompileHeaderWithTexture(name_header, OPTYPE, *nametex);
	CompileHeader(sname_header, TRTYPE);
	
	kos_texture_t* ibgtex = load_png_texture("/rd/blob.png");
	kos_texture_t* ftortex = load_png_texture("/rd/ftorus.png");
	kos_texture_t* sftortex = load_png_texture("/rd/ftorus0.png");
	
	CompileHeaderWithTexture(ftorus_header, TRTYPE, *ftortex);
	CompileHeaderWithTexture(ftorus_shadow_header, TRTYPE, *sftortex);
	CompileHeaderWithTexture(ibg_header, OPTYPE, *ibgtex);
	
	Quaternion iq;
	introS.scale(1.8f, 1.8f, 1.8f); 
	introT.translate(0, 0, 0);
	introR.quaternionToMatrix(iq);
}

void IntroRun()
{
//0047
//0564
//1081
	static int timerList[]=
	{
		51,//564,
		51,//1081, 
		34,//1424,
		51,//1943,
		51,//2460
	};
	
	static Vector camPoses[]=
	{
		{4.0f,-2.9f, 3.0f},
		{4.0f, 3.9f, 3.0f},
		{3.0f, 7.9f, 3.0f},
		{7.0f,-6.9f, 3.0f},
		{7.0f, -4.9f, 3.0f},
		{8.0f, 3.9f, 3.0f},
		{12.0f, 0, 3.0f}
	};
	
	/*USE DIFFERENT CAMERA ANGLES (VECTOR ARRAY LIST)*/
	
	static float rt = -0.5f;
	static uint64_t lerpTimer = 4*10+timer_ms_gettime64();
	static int lerpCounter = 0;

	rt += 0.001f;
	Vector v(3, 1, 0);
	
	static Matrix ShadowScale;
	ShadowScale.scale(1.0f, 1.0f, 0.0f);

	static Vector lastPos(12.0f, 0, 3.0f);
	static Vector startPos(9.0f, 4.0f, 6.0f); 
	static float curpos = 0;
	if (curpos > 1.0f) curpos = 1.0f; 	
	Vector Pos = Vector::Lerp(startPos, camPoses[lerpCounter], curpos);
	float d = Vector::Distance(Pos, lastPos) * 0.3f;
	curpos += 0.03f * d;//((d > 1.0f) ? 1.0f : d);
;
	if (lerpTimer < timer_ms_gettime64() && lerpCounter <= 5)
	{
		lerpTimer += timerList[lerpCounter]*10;// + timer_ms_gettime64();
		lerpCounter++; 
		curpos = 0;
	}

	introCam.lookAt(v); 

	Quaternion q = Quaternion::Euler(0, 110, 45);

	introCam.position = q * Vector::Forward() * Pos.x + Vector::Right()* Pos.z + Vector::Up() * Pos.y ;//+ Vector::Up() * -2.5f; 
	introCam.applyView();

    pvr_wait_ready();
   	pvr_scene_begin();
   	pvr_list_begin(PVR_LIST_OP_POLY);
   	
   		introT.translate(-rt*10.5f, 0, 0);

   		introR.rotateX(6); 

   		mat_identity();
		mat_apply(&proj.elem);
    	mat_apply(&Camera::view.elem); 
    	mat_apply(&introT.elem); 
    	mat_apply(&introR.elem);
    	mat_apply(&introS.elem);
    	
   		render_name();
   		
   		introT.translate(0, 0, 21.0f);	
		introS.scale(11.0f, 11.0f, 0.0f);
		
   		mat_identity();
		mat_apply(&proj.elem);
    	mat_apply(&introT.elem); 
    	mat_apply(&introS.elem);
    	
    	render_name_ibg(curpos,curpos);
   		introS.scale(1.8f, 1.8f, 1.8f); 
   	pvr_list_finish();
   	pvr_list_begin(PVR_LIST_TR_POLY);
   		
   		introT.translate(-rt*8.0f+0.75f, -2.5f, 0);

   		mat_identity();
   		mat_apply(&ShadowScale.elem);
		mat_apply(&proj.elem);
    	mat_apply(&Camera::view.elem); 
    	mat_apply(&introT.elem); 
    	mat_apply(&introR.elem);
    	
   		render_name_shadow();
   		
   		introT.translate(0, 0, 0);
	   	introR.rotateX(rt);
   		mat_identity();
		mat_apply(&proj.elem);
    	mat_apply(&Camera::view.elem); 
    	mat_apply(&introT.elem); 
    	mat_apply(&introR.elem);
    	mat_apply(&introS.elem);
    	
   		render_ftorus();
   	
   		introT.translate(5, 0, 0);	
   			
   		mat_identity();
		mat_apply(&proj.elem);
    	mat_apply(&Camera::view.elem); 
    	mat_apply(&introT.elem); 
    	mat_apply(&introR.elem);
    	mat_apply(&introS.elem);
    	
    	render_ftorus();
    	
    	introT.translate(10, 0, 0);	
   			
   		mat_identity();
		mat_apply(&proj.elem);
    	mat_apply(&Camera::view.elem); 
    	mat_apply(&introT.elem); 
    	mat_apply(&introR.elem);
    	mat_apply(&introS.elem);
    	
    	render_ftorus();
    	
    	ShadowScale.scale(1.0f, 1.0f, 0.0f);
    	
    	introT.translate(2, -2, 5.0f);	
		mat_identity();
		mat_apply(&ShadowScale.elem);
		mat_apply(&proj.elem);
    	mat_apply(&Camera::view.elem); 
    	mat_apply(&introT.elem); 
    	mat_apply(&introR.elem);
    	render_ftorus_shadow();
    	
    	introT.translate(7, -2, 5.0f);	
		mat_identity();
		mat_apply(&ShadowScale.elem);
		mat_apply(&proj.elem);
    	mat_apply(&Camera::view.elem); 
    	mat_apply(&introT.elem); 
    	mat_apply(&introR.elem);
    	render_ftorus_shadow();
    	
    	introT.translate(12, -2, 5.0f);	
		mat_identity();
		mat_apply(&ShadowScale.elem);
		mat_apply(&proj.elem);
    	mat_apply(&Camera::view.elem); 
    	mat_apply(&introT.elem); 
    	mat_apply(&introR.elem);
    	render_ftorus_shadow();
    	
   	pvr_list_finish();
	pvr_scene_finish(); 
}
