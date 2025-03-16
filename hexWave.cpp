#include "sequences.h"
#include "ajHeaders.h" 

#include "geometry/hex_ply.h" 
#include "input.h" 

pvr_poly_hdr_t hex_header; 
static int stopTimer; 

void render_hex_cylinder(float lerpValue)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.3f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	float rr = 0.4f, gg = 1.0f, bb = 0.6f, aa = lerpValue;
	vec3f_t v __attribute__((aligned(32)));
	
	static Vector light = Vector::Normalize(Vector(-1.0f, -0.5, -1.0f));
	static float x, y; 
	static Vector l;
	x += 0.008f; 
	y += 0.005f; 
	l = Quaternion::Euler(0, x, y) * light; 

	static Vector normal; 
	static float intensity; 
	
	pvr_prim(&hex_header, sizeof(hex_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < HEX_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(hex_verts[hex_faces[i].x].x,
							hex_verts[hex_faces[i].x].y,
							hex_verts[hex_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		normal.x = hex_norms[hex_faces[i].x].x; 
		normal.y = hex_norms[hex_faces[i].x].y; 
		normal.z = hex_norms[hex_faces[i].x].z; 

		intensity = MAX(ABS(Vector::Dot(normal, l)), 0);
		alpha = MIN(0.3f, aa * intensity);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = hex_uvs[hex_faces[i].x].u; 
		tvert->v = hex_uvs[hex_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(hex_verts[hex_faces[i].y].x,
							hex_verts[hex_faces[i].y].y,
							hex_verts[hex_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = hex_uvs[hex_faces[i].y].u; 
		tvert->v = hex_uvs[hex_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(hex_verts[hex_faces[i].z].x,
							hex_verts[hex_faces[i].z].y,
							hex_verts[hex_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
			
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = hex_uvs[hex_faces[i].z].u; 
		tvert->v = hex_uvs[hex_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void HexWaveInit()
{
	CompileHeader(hex_header, TRTYPE);
	stopTimer = 11042 * 10 + timer_ms_gettime64();
}

void HexWaveRun()
{
	static Matrix hexT, hexS, hexR; 
	static const int hexAmount = 9;
	static Camera hexCam; 
	static Vector lookTarget(6.6f, 3.0f, 25.0f);
	static float sweep = 0; 
	static float cursweep = 0; 
	static int snareTimer = timer_ms_gettime64();
	static float lerpValue = 0;
	static float lerpAlpha = 0;
	static int timerContext = 0; 
	static float timeList[]
	{
		85, 
		53
	};
	
	if (stopTimer > timer_ms_gettime64())
	{
		if (snareTimer < timer_ms_gettime64())
		{
			lerpValue = 0; 
			snareTimer += timeList[timerContext]*10; 
			timerContext ^= 1; 
		}
		
		lerpValue += 0.04f; 
		if (lerpValue > 1.0f) lerpValue = 1.0f;
		lerpAlpha = lerpf(0.13f, 0.8f, lerpValue);
	}else 
	{
		lerpAlpha -= 0.007f;
		if (lerpAlpha < 0) lerpAlpha = 0; 
	}
	
	hexCam.lookAt(lookTarget);
	hexCam.position = Vector(-26.13f, 15.0f, 12.87f);
	hexCam.applyView();

	    pvr_wait_ready();
       	pvr_scene_begin();     	
     	
       	pvr_list_begin(PVR_LIST_TR_POLY);
  
   		for(int i = 0; i < hexAmount+5; ++i)
   		{		
   			cursweep += 0.1f;
   			sweep = 0;
   			for(int j = 0; j < hexAmount; ++j)
   			{
   				sweep += cursweep*0.01f;
				hexT.translate(-7.7f+(j*2), 5*fsin((sweep+i)), 10.0f+((i*2)+0.5f));
		   		mat_identity();
			   	mat_apply(&proj.elem);
			   	mat_apply(&Camera::view.elem); 
			   	mat_apply(&hexT.elem); 
		   	
			   	render_hex_cylinder(lerpAlpha);
		   	}
		}
       	pvr_list_finish();
		pvr_scene_finish();
}
