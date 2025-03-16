#include "sequences.h"
#include "ajHeaders.h" 
#include "geometry/plane_ply.h" 
#include "input.h" 

pvr_poly_hdr_t trail_origin_header; 
pvr_poly_hdr_t trail_tail_header;

#define PARTICLE_USED 1
#define PARTICLE_FREE 0 

struct Particle
{
	int isInit;
	int flag; 
	float scale; 
	Vector Pos; 
	int life;
}; 

static const int originAmount = 7;
static const int tailAmount = 40;
static Particle tails[originAmount][tailAmount];
static uint64_t fadeTimer; 

static Vector OPos[originAmount];

void move_vector(Vector & v1, const Vector & v2, float l)
{
#if 1
if (l <= 0.00f || l > 1.00f) return; 
    Vector a(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z); 
	float mag = a.magnitude();
	mag = l * frsqrt(mag*mag); 

	v1.x = v1.x + (a.x * mag);
	v1.y = v1.y + (a.y * mag);
	v1.z = v1.z + (a.z * mag);

#else 
	if (l <= 0.00f || l > 1.00f) return; 
	
    register float __ax __asm__("fr0") = (v2.x - v1.x); 
    register float __ay __asm__("fr1") = (v2.y - v1.y); 
    register float __az __asm__("fr2") = (v2.z - v1.z); 
    register float __v1 __asm__("fr4") = (v1.x); 
    register float __v2 __asm__("fr5") = (v1.y); 
    register float __v3 __asm__("fr6") = (v1.z); 
    register float __l __asm__("fr7") = (l); 
       //printf("a %f %f %f\n", __ax, __ay, __az); 
        __asm__ volatile("\n"
				"fldi0 fr3 \n"
				"fmov fr7, fr8 \n"
				"fldi0 fr7 \n"
				"fipr  fv0,fv0 \n" 
				"fsqrt fr3 \n"
				"fmul fr3, fr3 \n"
				"fsrra fr3 \n"
				"fmul fr3, fr0 \n"
				"fmul fr3, fr1 \n"
				"fmul fr3, fr2 \n"
				"fmul fr8, fr0 \n"
				"fmul fr8, fr1 \n"
				"fmul fr8, fr2 \n"
				"fadd fr0, fr4 \n"
				"fadd fr1, fr5 \n"
				"fadd fr2, fr6 \n"
		: "+f" (__v1), "+f" (__v2), "+f" (__v3)
		: "f" (__ax), "f" (__ay), "f" (__az), "f" (__l) 
		: "fr3" 	
	);

	v1.x = __v1;
	v1.y = __v2; 
	v1.z = __v3; 
#endif
}

void render_origin_trail(int tail, float alpha = 1.0f)
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;

	static float r = 0.8f, g = 0.8f, b = 0.8f;
	vec3f_t v __attribute__((aligned(32)));

	if (tail){pvr_prim(&trail_tail_header, sizeof(trail_tail_header));}
	else pvr_prim(&trail_origin_header, sizeof(trail_origin_header));
	
	pvr_dr_init(&dr_state);
	

		mat_trans_single3_nomod(plane_verts[2].x,
							plane_verts[2].y,
							plane_verts[2].z,
							v.x, v.y, v.z);
							
		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;
						  	
		tvert->u = plane_uvs[2].u; 
		tvert->v = plane_uvs[2].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(plane_verts[1].x,
							plane_verts[1].y,
							plane_verts[1].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = plane_uvs[1].u; 
		tvert->v = plane_uvs[1].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(plane_verts[0].x,
							plane_verts[0].y,
							plane_verts[0].z,
							v.x, v.y, v.z);
						
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = plane_uvs[0].u; 
		tvert->v = plane_uvs[0].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert);
		
mat_trans_single3_nomod(plane_verts[3].x,
						plane_verts[3].y,
						plane_verts[3].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);
		tvert->oargb = 0;								

		tvert->u = plane_uvs[3].u; 
		tvert->v = plane_uvs[3].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	pvr_dr_finish();
}

void TrailInit()
{
	kos_texture_t* otrail = load_png_texture("/rd/otrail.png");
	kos_texture_t* ttrail = load_png_texture("/rd/ttrail.png");
	
	CompileHeaderWithTexture(trail_origin_header, TRTYPE, *otrail);
	CompileHeaderWithTexture(trail_tail_header, TRTYPE, *ttrail);
	
	for (int i = 0; i < originAmount; ++i)
	{
		for (int j = 0; j < tailAmount; ++j)
		{
			tails[i][j].isInit = 0;
			tails[i][j].flag = PARTICLE_FREE; 
			tails[i][j].scale = 1.0f; 
			tails[i][j].Pos  = Vector::Zero(); 
			tails[i][j].life = 10;
		}
	}
	
	fadeTimer = 12340 * 10 + timer_ms_gettime64();
	
	Quaternion rotAmount; 
	Quaternion::fAxisAngle(rotAmount, 0, 0, 1, 20.0f);

	for (int i = 0; i < originAmount; ++i)
	{	
		OPos[i] = rotAmount * Vector::Forward(); 
		rotAmount *= rotAmount; 
	}
}

void TrailRun()
{
	static Matrix traT, traS, traR;
	static Vector followTarget; 
	static Quaternion q; 
	static Vector originPos;
	static Vector tailPos;  
	static float qx, qy, qz; 
	static float fadeout = 1.0; 

	if (fadeTimer < timer_ms_gettime64())
	{
		fadeout += 0.01f;
		if (fadeout > 1.0f) fadeout = 1.0f; 
	}else 
	{
		fadeout -= 0.01f;
		if (fadeout < 0.0f) fadeout = 0.0f; 
	}
	
	Vector dirOffset; 
	qx += 0.01f; 
	qy += 0.005f;
	qz += 0.03f; 
	
	followTarget = Vector(9*fsin(qx*0.1f), 9*fcos(qx*0.3f), 15+(9*fsin(qx*0.2f)));
	
	pvr_wait_ready();
   	pvr_scene_begin();     	

   	pvr_list_begin(PVR_LIST_TR_POLY); 
   	int i =0;
   	//unwrapped loop, idk if it helped, 
   	//too lazy to test so it's staying like that
	//for(int i = 0; i < originAmount; ++i)
	{		
		qx += 0.01f; 
		qy += 0.005f;
		qz += 0.03f;
		 
		Quaternion::fEuler(q, qx*20.7f, qy*18.3f, qz*11.7f);

		originPos = followTarget - q * OPos[i] * 6.0f;
		
		traT.translate(originPos.x, originPos.y, 15.0f+originPos.z);
		
   		mat_identity();
	   	mat_apply(&proj.elem);
	   	mat_apply(&traT.elem); 
	   	
	   	render_origin_trail(0, 1.0f - fadeout);
	   	
	   	float dis; 
	   	for(int j = 0; j < tailAmount; ++j)
	   	{
	   		if (tails[i][j].isInit == 0)
	   		{
	   			tails[i][j].Pos = originPos; 
	   			tails[i][j].isInit = 1;
	   		}
	   	
	   		if (j <= 0)
	   		{
				move_vector(tails[i][j].Pos, originPos, (Vector::SqrDistance(originPos, tails[i][j].Pos)*0.3f));
				tails[i][j].scale = 1.0f;	   		
	   		}else 
	   		{
	   			dis = MIN(1.0f, (Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*0.65f));
	   			move_vector(tails[i][j].Pos, tails[i][j-1].Pos, dis);
	   			 if (j != tailAmount)
	   				tails[i][j].scale = (float)(tailAmount - j) / (float)tailAmount;//(Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*2.97f);
	   			else 
	   				tails[i][j].scale = 0;
	   		}
	   		
	   		//if (fadeTimer < timer_ms_gettime64())
	   		{
	   			tails[i][j].scale -= fadeout;
	   			if (tails[i][j].scale < 0) tails[i][j].scale = 0;
	   		}
	   		
	   		traT.translate(tails[i][j].Pos.x, tails[i][j].Pos.y, 15.0f+tails[i][j].Pos.z); 

	   		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&traT.elem); 
		   	
		   	render_origin_trail(1, tails[i][j].scale);
	   	}
	}//loop ends here
	i=1;
	{		
		qx += 0.01f; 
		qy += 0.005f;
		qz += 0.03f;

		Quaternion::fEuler(q, qx*20.7f, qy*18.3f, qz*11.7f);

		originPos = followTarget - q * OPos[i] * 6.0f;
		
		traT.translate(originPos.x, originPos.y, 15.0f+originPos.z);
		
   		mat_identity();
	   	mat_apply(&proj.elem);
	   	mat_apply(&traT.elem); 
	   	
	   	render_origin_trail(0, 1.0f - fadeout);
	   	
	   	float dis; 
	   	for(int j = 0; j < tailAmount; ++j)
	   	{
	   		if (tails[i][j].isInit == 0)
	   		{
	   			tails[i][j].Pos = originPos; 
	   			tails[i][j].isInit = 1;
	   		}
	   	
	   		if (j <= 0)
	   		{
				move_vector(tails[i][j].Pos, originPos, (Vector::SqrDistance(originPos, tails[i][j].Pos)*0.3f));
				tails[i][j].scale = 1.0f;	   		
	   		}else 
	   		{
	   			dis = MIN(1.0f, (Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*0.65f));

	   			move_vector(tails[i][j].Pos, tails[i][j-1].Pos, dis);
	   			 if (j != tailAmount)
	   				tails[i][j].scale = (float)(tailAmount - j) / (float)tailAmount;//(Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*2.97f);
	   			else 
	   				tails[i][j].scale = 0;
	   		}
	   		
	   		//if (fadeTimer < timer_ms_gettime64())
	   		{
	   			tails[i][j].scale -= fadeout;
	   			if (tails[i][j].scale < 0) tails[i][j].scale = 0;
	   		}

	   		traT.translate(tails[i][j].Pos.x, tails[i][j].Pos.y, 15.0f+tails[i][j].Pos.z); 
	   		
	   		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&traT.elem); 
		   	
		   	render_origin_trail(1, tails[i][j].scale);
	   	}
	}
	i=2;
	{		
		qx += 0.01f; 
		qy += 0.005f;
		qz += 0.03f;
		 
		Quaternion::fEuler(q, qx*20.7f, qy*18.3f, qz*11.7f);

		originPos = followTarget - q * OPos[i] * 6.0f;
		
		traT.translate(originPos.x, originPos.y, 15.0f+originPos.z);
		
   		mat_identity();
	   	mat_apply(&proj.elem);
	   	mat_apply(&traT.elem); 
	   	
	   	render_origin_trail(0, 1.0f - fadeout);
	   	 
	   	float dis; 
	   	for(int j = 0; j < tailAmount; ++j)
	   	{
	   		if (tails[i][j].isInit == 0)
	   		{
	   			tails[i][j].Pos = originPos; 
	   			tails[i][j].isInit = 1;
	   		}
	   	
	   		if (j <= 0)
	   		{
				move_vector(tails[i][j].Pos, originPos, (Vector::SqrDistance(originPos, tails[i][j].Pos)*0.3f));
				tails[i][j].scale = 1.0f;	   		
	   		}else 
	   		{
	   			dis = MIN(1.0f, (Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*0.65f));

	   			move_vector(tails[i][j].Pos, tails[i][j-1].Pos, dis);
	   			 if (j != tailAmount)
	   				tails[i][j].scale = (float)(tailAmount - j) / (float)tailAmount;//(Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*2.97f);
	   			else 
	   				tails[i][j].scale = 0;
	   		}
	   		
	   		//if (fadeTimer < timer_ms_gettime64())
	   		{
	   			tails[i][j].scale -= fadeout;
	   			if (tails[i][j].scale < 0) tails[i][j].scale = 0;
	   		}

	   		traT.translate(tails[i][j].Pos.x, tails[i][j].Pos.y, 15.0f+tails[i][j].Pos.z); 
	   		
	   		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&traT.elem);
		   	
		   	render_origin_trail(1, tails[i][j].scale);
	   	}
	}
	i=3;
	{		
		qx += 0.01f; 
		qy += 0.005f;
		qz += 0.03f;
		 
		Quaternion::fEuler(q, qx*20.7f, qy*18.3f, qz*11.7f);
		
		originPos = followTarget - q * OPos[i] * 6.0f;
		
		traT.translate(originPos.x, originPos.y, 15.0f+originPos.z);
		
   		mat_identity();
	   	mat_apply(&proj.elem);
	   	mat_apply(&traT.elem); 
	   	
	   	render_origin_trail(0, 1.0f - fadeout);
	   	
	   	float dis; 
	   	for(int j = 0; j < tailAmount; ++j)
	   	{
	   		if (tails[i][j].isInit == 0)
	   		{
	   			tails[i][j].Pos = originPos; 
	   			tails[i][j].isInit = 1;
	   		}
	   	
	   		if (j <= 0)
	   		{
				move_vector(tails[i][j].Pos, originPos, (Vector::SqrDistance(originPos, tails[i][j].Pos)*0.3f));
				tails[i][j].scale = 1.0f;	   		
	   		}else 
	   		{
	   			dis = MIN(1.0f, (Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*0.65f));

	   			move_vector(tails[i][j].Pos, tails[i][j-1].Pos, dis);
	   			 if (j != tailAmount)
	   				tails[i][j].scale = (float)(tailAmount - j) / (float)tailAmount;//(Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*2.97f);
	   			else 
	   				tails[i][j].scale = 0;  
	   		}
	   		
	   		//if (fadeTimer < timer_ms_gettime64())
	   		{
	   			tails[i][j].scale -= fadeout;
	   			if (tails[i][j].scale < 0) tails[i][j].scale = 0;
	   		}

	   		traT.translate(tails[i][j].Pos.x, tails[i][j].Pos.y, 15.0f+tails[i][j].Pos.z); 
	   		
	   		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&traT.elem);
		   	
		   	render_origin_trail(1, tails[i][j].scale);
	   	}
	}
	i=4;
	{		
		qx += 0.01f; 
		qy += 0.005f;
		qz += 0.03f;
		 
		Quaternion::fEuler(q, qx*20.7f, qy*18.3f, qz*11.7f);

		originPos = followTarget - q * OPos[i] * 6.0f;
		
		traT.translate(originPos.x, originPos.y, 15.0f+originPos.z);
		
   		mat_identity();
	   	mat_apply(&proj.elem);
	   	mat_apply(&traT.elem);
	   	
	   	render_origin_trail(0, 1.0f - fadeout);
	   	
	   	float dis; 
	   	for(int j = 0; j < tailAmount; ++j)
	   	{
	   		if (tails[i][j].isInit == 0)
	   		{
	   			tails[i][j].Pos = originPos; 
	   			tails[i][j].isInit = 1;
	   		}
	   	
	   		if (j <= 0)
	   		{
				move_vector(tails[i][j].Pos, originPos, (Vector::SqrDistance(originPos, tails[i][j].Pos)*0.3f));
				tails[i][j].scale = 1.0f;	   		
	   		}else 
	   		{
	   			dis = MIN(1.0f, (Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*0.65f));

	   			move_vector(tails[i][j].Pos, tails[i][j-1].Pos, dis);
	   			 if (j != tailAmount)
	   				tails[i][j].scale = (float)(tailAmount - j) / (float)tailAmount;//(Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*2.97f);
	   			else 
	   				tails[i][j].scale = 0;
	   		}
	   		
	   		//if (fadeTimer < timer_ms_gettime64())
	   		{
	   			tails[i][j].scale -= fadeout;
	   			if (tails[i][j].scale < 0) tails[i][j].scale = 0;
	   		}

	   		traT.translate(tails[i][j].Pos.x, tails[i][j].Pos.y, 15.0f+tails[i][j].Pos.z); 
	   		
	   		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&traT.elem);
		   	
		   	render_origin_trail(1, tails[i][j].scale);
	   	}
	}
	i=5;
	{		
		qx += 0.01f; 
		qy += 0.005f;
		qz += 0.03f;
		 
		Quaternion::fEuler(q, qx*20.7f, qy*18.3f, qz*11.7f);

		originPos = followTarget - q * OPos[i] * 6.0f;
		
		traT.translate(originPos.x, originPos.y, 15.0f+originPos.z);
		
   		mat_identity();
	   	mat_apply(&proj.elem);
	   	mat_apply(&traT.elem);
	   	
	   	render_origin_trail(0, 1.0f - fadeout);
	   	
	   	float dis; 
	   	for(int j = 0; j < tailAmount; ++j)
	   	{
	   		if (tails[i][j].isInit == 0)
	   		{
	   			tails[i][j].Pos = originPos; 
	   			tails[i][j].isInit = 1;
	   		}
	   	
	   		if (j <= 0)
	   		{
				move_vector(tails[i][j].Pos, originPos, (Vector::SqrDistance(originPos, tails[i][j].Pos)*0.3f));
				tails[i][j].scale = 1.0f;	   		
	   		}else 
	   		{
	   			dis = MIN(1.0f, (Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*0.65f));

	   			move_vector(tails[i][j].Pos, tails[i][j-1].Pos, dis);
	   			 if (j != tailAmount)
	   				tails[i][j].scale = (float)(tailAmount - j) / (float)tailAmount;//(Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*2.97f);
	   			else 
	   				tails[i][j].scale = 0;
	   		}
	   		
	   		//if (fadeTimer < timer_ms_gettime64())
	   		{
	   			tails[i][j].scale -= fadeout;
	   			if (tails[i][j].scale < 0) tails[i][j].scale = 0;
	   		}

	   		traT.translate(tails[i][j].Pos.x, tails[i][j].Pos.y, 15.0f+tails[i][j].Pos.z); 
	   		
	   		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&traT.elem);
		   	
		   	render_origin_trail(1, tails[i][j].scale);
	   	}
	}
	i=6;
	{		
		qx += 0.01f; 
		qy += 0.005f;
		qz += 0.03f;
		 
		Quaternion::fEuler(q, qx*20.7f, qy*18.3f, qz*11.7f);

		originPos = followTarget - q * OPos[i] * 6.0f;
		
		traT.translate(originPos.x, originPos.y, 15.0f+originPos.z);
		
   		mat_identity();
	   	mat_apply(&proj.elem);
	   	mat_apply(&traT.elem); 
	   	
	   	render_origin_trail(0, 1.0f - fadeout);
	   	
	   	float dis; 
	   	for(int j = 0; j < tailAmount; ++j)
	   	{
	   		if (tails[i][j].isInit == 0)
	   		{
	   			tails[i][j].Pos = originPos; 
	   			tails[i][j].isInit = 1;
	   		}
	   	
	   		if (j <= 0)
	   		{
				move_vector(tails[i][j].Pos, originPos, (Vector::SqrDistance(originPos, tails[i][j].Pos)*0.3f));
				tails[i][j].scale = 1.0f;	   		
	   		}else 
	   		{
	   			dis = MIN(1.0f, (Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*0.65f));

	   			move_vector(tails[i][j].Pos, tails[i][j-1].Pos, dis);
	   			 if (j != tailAmount)
	   				tails[i][j].scale = (float)(tailAmount - j) / (float)tailAmount;//(Vector::SqrDistance(tails[i][j-1].Pos, tails[i][j].Pos)*2.97f);
	   			else 
	   				tails[i][j].scale = 0;
	   		}
	   		
	   		//if (fadeTimer < timer_ms_gettime64())
	   		{
	   			tails[i][j].scale -= fadeout;
	   			if (tails[i][j].scale < 0) tails[i][j].scale = 0;
	   		}

	   		traT.translate(tails[i][j].Pos.x, tails[i][j].Pos.y, 15.0f+tails[i][j].Pos.z); 
	   		
	   		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&traT.elem);
		   	
		   	render_origin_trail(1, tails[i][j].scale);
	   	}
	}
   	pvr_list_finish();
	pvr_scene_finish();
}
