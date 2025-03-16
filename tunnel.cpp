#include "sequences.h"
#include "ajHeaders.h" 
#include "geometry/tunnel_ply.h"
#include "geometry/fcube_ply.h"
#include "input.h" 
#include <cmath>

pvr_poly_hdr_t tunnel1_header;
pvr_poly_hdr_t tunnel2_header;
pvr_poly_hdr_t cube_header;

Quaternion cubeNRot; 
static uint64_t swapTimer; 
static uint64_t animateTimer; 

void render_tunnel(int tun1, int swap, float litLerp = 1.0f)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 1.0f; 

	vec3f_t v __attribute__((aligned(32)));
	static float uOffset, vOffset; 
	static float lit;
	static float lit_offset = 0.8f;
	static float lit_r = 0.6f; 
	static float bright = 1.3f; 
	
	if (swap)
	{
		lit_offset = 0.8f;
		lit_r = 0.6f; 
		bright = 1.7f; 	
	}else
	{
		lit_offset = 0.8f;
		lit_r = 0.6f; 
		bright = 1.3f; 
	} 
	
	vOffset += 0.005f; 
	//if (uOffset >= 1.0f) uOffset = 0; 
	if (vOffset >= 1.0f) vOffset = 0; 
	
	if (tun1)
	{	
		static float ruOffset = 0; 
		ruOffset -= 0.004f; 
		uOffset = ruOffset; 
		if (ruOffset <= -1.0f) ruOffset = 0; 
		pvr_prim(&tunnel1_header, sizeof(tunnel1_header));
	}
	else 
	{
		static float ruOffset = 0;
		alpha = 0.7f;
		ruOffset += 0.0078f; 
		uOffset = ruOffset; 
		if (ruOffset >= 1.0f) ruOffset = 0; 
		pvr_prim(&tunnel2_header, sizeof(tunnel2_header));
	}
	
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < TUNNEL_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(tunnel_verts[tunnel_faces[i].x].x,
							tunnel_verts[tunnel_faces[i].x].y,
							tunnel_verts[tunnel_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 

		if (tun1) lit = litLerp;
		else lit = MAX(bright * (lit_offset + (tunnel_verts[tunnel_faces[i].x].z * lit_r)), 1.0f); 
		tvert->argb = PVR_PACK_COLOR(alpha, lit, lit, lit);
		tvert->oargb = 0;
						  	
		tvert->u = tunnel_uvs[tunnel_faces[i].x].u + uOffset; 
		tvert->v = tunnel_uvs[tunnel_faces[i].x].v + vOffset;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(tunnel_verts[tunnel_faces[i].y].x,
							tunnel_verts[tunnel_faces[i].y].y,
							tunnel_verts[tunnel_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		
		if (tun1) lit = litLerp;
		else lit = MAX(bright * (lit_offset + (tunnel_verts[tunnel_faces[i].y].z * lit_r)), 1.0f);

		tvert->argb = PVR_PACK_COLOR(alpha, lit, lit, lit);

		tvert->oargb = 0;								

		tvert->u = tunnel_uvs[tunnel_faces[i].y].u + uOffset; 
		tvert->v = tunnel_uvs[tunnel_faces[i].y].v + vOffset;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(tunnel_verts[tunnel_faces[i].z].x,
							tunnel_verts[tunnel_faces[i].z].y,
							tunnel_verts[tunnel_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 

		if (tun1) lit = litLerp;
		else lit = MAX(bright * (lit_offset + (tunnel_verts[tunnel_faces[i].z].z * lit_r)), 1.0f); 
		tvert->argb = PVR_PACK_COLOR(alpha, lit, lit, lit);
		tvert->oargb = 0;								

		tvert->u = tunnel_uvs[tunnel_faces[i].z].u + uOffset; 
		tvert->v = tunnel_uvs[tunnel_faces[i].z].v + vOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void render_cube()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.3f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	float rr = 0.4f, gg = 0.6f, bb = 1.0f, aa = 0.8f;
	vec3f_t v __attribute__((aligned(32)));
	
	static Vector light = Vector::Normalize(Vector(-1.0f, -0.5, -1.0f));
	static float x, y; 
	static Vector l;
	x += 0.8f; 
	y += 0.5f; 
	l = Quaternion::Euler(0, x, y) * light; 

	static Vector normal; 
	static float intensity; 
	
	pvr_prim(&cube_header, sizeof(cube_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < FCUBE_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(fcube_verts[fcube_faces[i].x].x,
							fcube_verts[fcube_faces[i].x].y,
							fcube_verts[fcube_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		normal.x = fcube_norms[fcube_faces[i].x].x; 
		normal.y = fcube_norms[fcube_faces[i].x].y; 
		normal.z = fcube_norms[fcube_faces[i].x].z; 
		normal = cubeNRot * normal; 

		intensity = MAX(ABS(Vector::Dot(normal, l)), 0);
		alpha = MIN(0.3f, aa * intensity);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = fcube_uvs[fcube_faces[i].x].u; 
		tvert->v = fcube_uvs[fcube_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(fcube_verts[fcube_faces[i].y].x,
							fcube_verts[fcube_faces[i].y].y,
							fcube_verts[fcube_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
			
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = fcube_uvs[fcube_faces[i].y].u; 
		tvert->v = fcube_uvs[fcube_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(fcube_verts[fcube_faces[i].z].x,
							fcube_verts[fcube_faces[i].z].y,
							fcube_verts[fcube_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
				
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = fcube_uvs[fcube_faces[i].z].u; 
		tvert->v = fcube_uvs[fcube_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void TunnelInit()
{
	kos_texture_t* tun1tex = load_png_texture("/rd/tunnel.png");
	kos_texture_t* tun2tex = load_png_texture("/rd/tunnel2.png");
	CompileHeaderWithTexture(tunnel1_header, OPTYPE, *tun1tex);
	CompileHeaderWithTexture(tunnel2_header, TRTYPE, *tun2tex);
	CompileHeader(cube_header, TRTYPE);
	
	swapTimer = 5800 * 10 + timer_ms_gettime64();
	animateTimer = 6350 * 10 + timer_ms_gettime64();
}

void TunnelRun()
{
		static Matrix tunT, tunS, tunR; 
       	static float r; 
       	static float litlerp; 
       	static float lerpValue = 0;
       	static float clerpValue = 0; 
       	static int i = 0; 
		static float yy[3]={0.5f, 0.0f, -0.5f};
		static float xx[3]={-0.3f, 0.3f, -0.3f};
		static float ss; 
		static int renderCubes = 1; 
		static uint64_t snareTimer = timer_ms_gettime64();
		static uint64_t bassTimer = timer_ms_gettime64();
		static int timerContext;  
		static float timeList[]
		{
			85, 
			53
		};
		
		litlerp = lerpf(1.0f, 0.0f, lerpValue);
		ss = lerpf(0.3f, 0.18f, clerpValue);
		lerpValue += 0.03f;
		clerpValue += 0.02f;
		 
		if (lerpValue > 1.0f) lerpValue = 1.0f; 
		if (clerpValue > 1.0f) clerpValue = 1.0f; 

		if (animateTimer > timer_ms_gettime64())
		{
			if (swapTimer > timer_ms_gettime64())
			{		
				if (snareTimer < timer_ms_gettime64())
				{
					lerpValue = 0; 
					snareTimer += 70*10;
				}
				
				if (bassTimer < timer_ms_gettime64())
				{
					clerpValue = 0; 
					bassTimer += timeList[timerContext]*10;
					timerContext ^= 1; 
				}
			}else
			{
				renderCubes = 0;
				i = 1; 
				
				if (bassTimer < timer_ms_gettime64())
				{
					lerpValue = 0; 
					bassTimer += timeList[timerContext]*10;
					timerContext ^= 1; 
				}
			}
		}
		else 
		{
			lerpValue = 0;
		}
		
      	if (gInput.buttonDown(BN_CROSS))
      	{
       		i ^= 1; 
       	}
       	
       	if (gInput.buttonDown(BN_SQUARE))
       	{
       		lerpValue = 0;
       	}
       	   	
       	r +=0.01f;
       	cubeNRot = Quaternion::Euler(r*65.0f, r*55.3f, 0);
       	     	
       	tunT.translate(0.0f, 0.0f, 1.6f);
       	tunR.rotateZ(r);
       	
       	pvr_wait_ready();
       	pvr_scene_begin();
       	pvr_list_begin(PVR_LIST_OP_POLY);
       		tunS.scale(2.0f, 2.0f, 2.0f);
       	    mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&tunT.elem); 
		   	mat_apply(&tunR.elem);
		   	mat_apply(&tunS.elem);
		   	render_tunnel(1, 0, litlerp);
 #if 1 		   	
       	pvr_list_finish();
     	
       	pvr_list_begin(PVR_LIST_TR_POLY);
       		tunS.scale(1.8f, 1.8f, 1.8f);
       		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&tunT.elem); 
		   	mat_apply(&tunR.elem);
		   	mat_apply(&tunS.elem);       		     	
       		render_tunnel(0, i); 
       		
     	if (renderCubes)
     	{  		
       		tunR.quaternionToMatrix(cubeNRot);
       		tunS.scale(ss, ss, ss);
			for(int i = 0; i < 3; ++i)
			{       		
		   		tunT.translate(xx[i], yy[i], 1.0f);
		   		
		   		mat_identity();
			   	mat_apply(&proj.elem);
			   	mat_apply(&tunT.elem); 
			   	mat_apply(&tunR.elem);
			   	mat_apply(&tunS.elem);       		     	
		   		render_cube(); 
		   	}
       	}
  #endif     		
       	pvr_list_finish();
		pvr_scene_finish();
}
