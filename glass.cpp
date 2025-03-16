#include "sequences.h" 
#include "ajHeaders.h" 
#include "geometry/tsphere_ply.h" 
#include "input.h" 

pvr_poly_hdr_t glass_header; 

static uint64_t stopTimer; 

void render_glass(int dance)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	static float alpha = 1.0f; 
	static float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	vec3f_t tv __attribute__((aligned(32)));
	static float rv = 0; 
	static float ry = 0; 
	static float rz = 0; 
	static float freq = 0;
	static float brightness = 0.7f;

	pvr_prim(&glass_header, sizeof(glass_header));
	pvr_dr_init(&dr_state);
	
	if (dance)
	{
		rv+=0.1f; 
		ry+=0.06f;
		rz+=0.03f;
	}

	if (rv > 6.283f) rv = 0; 
	if (ry > 6.283f) ry = 0; 
	if (rz > 6.283f) rz = 0; 
	
	for(int i = 0; i < TSPHERE_FACE_COUNT; ++i)
	{
	
	r = (0.5f * (fsin((tsphere_norms[tsphere_faces[i].x].z)*freq - rv)+1.0f)) * brightness; 
	g = b = r; 

	
	tv.x = tsphere_verts[tsphere_faces[i].x].x + tsphere_norms[tsphere_faces[i].x].x * fcos((tsphere_norms[tsphere_faces[i].x].z)*freq - rv); 
	tv.y = tsphere_verts[tsphere_faces[i].x].y + tsphere_norms[tsphere_faces[i].x].y * fsin((tsphere_norms[tsphere_faces[i].x].x)*freq - ry); 
	tv.z = tsphere_verts[tsphere_faces[i].x].z + tsphere_norms[tsphere_faces[i].x].z * fsin((tsphere_norms[tsphere_faces[i].x].y)*freq - rz); 
	 
	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, 
						  r, 
						  g,
						  b);
		tvert->oargb = 0;
						  	
		tvert->u = tsphere_uvs[tsphere_faces[i].x].u; 
		tvert->v = tsphere_uvs[tsphere_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	tv.x = tsphere_verts[tsphere_faces[i].y].x + tsphere_norms[tsphere_faces[i].x].x * fcos((tsphere_norms[tsphere_faces[i].y].z)*freq - rv); 
	tv.y = tsphere_verts[tsphere_faces[i].y].y + tsphere_norms[tsphere_faces[i].x].y * fsin((tsphere_norms[tsphere_faces[i].y].x)*freq - ry); 
	tv.z = tsphere_verts[tsphere_faces[i].y].z + tsphere_norms[tsphere_faces[i].x].z * fsin((tsphere_norms[tsphere_faces[i].y].y)*freq - rz); 

	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, 
						  r, 
						  g,
						  b);
		tvert->oargb = 0;								

		tvert->u = tsphere_uvs[tsphere_faces[i].y].u; 
		tvert->v = tsphere_uvs[tsphere_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	tv.x = tsphere_verts[tsphere_faces[i].z].x + (tsphere_norms[tsphere_faces[i].x].x + fcos((tsphere_norms[tsphere_faces[i].z].z)*freq - rv)*0.2f); 
	tv.y = tsphere_verts[tsphere_faces[i].z].y + (tsphere_norms[tsphere_faces[i].x].y + fsin((tsphere_norms[tsphere_faces[i].z].x)*freq - ry)*0.2f); 
	tv.z = tsphere_verts[tsphere_faces[i].z].z + (tsphere_norms[tsphere_faces[i].x].z + fsin((tsphere_norms[tsphere_faces[i].z].y)*freq - rz)*0.2f); 
		
	mat_trans_single3_nomod(tv.x,
							tv.y,
							tv.z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, 
						  r, 
						  g,
						  b);
		tvert->oargb = 0;								

		tvert->u = tsphere_uvs[tsphere_faces[i].z].u; 
		tvert->v = tsphere_uvs[tsphere_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	freq = 10;
	pvr_dr_finish();
}

void GlassInit()
{
	CompileHeader(glass_header, OPTYPE);
	stopTimer = 4422 * 10 + timer_ms_gettime64();
}

void GlassRun()
{
		static Matrix glaT, glaS, glaR; 
       	static float r; 
       	static int i = 0; 
       	static uint64_t snareTimer = timer_ms_gettime64();
       	
       	if (gInput.buttonDown(BN_CROSS))
       		i ^= 1; 
       	
       	r +=0.01f;
       	
       	if (stopTimer > timer_ms_gettime64())
       	{
		   	if (snareTimer < timer_ms_gettime64())
			{
				snareTimer += 35*10; 
				i ^= 1;
			}
		}else
		{
			i = 1; 
		}
		
       	pvr_wait_ready();
       	pvr_scene_begin();
       	pvr_list_begin(PVR_LIST_OP_POLY);
       		glaT.translate(0.0f, 0.0f, 6.0f);
       		glaR.rotateY(r);
       		
       		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&glaT.elem); 
		   	mat_apply(&glaR.elem);
		   	mat_apply(&glaS.elem);
       		     	
       		render_glass(i);
       	pvr_list_finish();
		pvr_scene_finish();
}
