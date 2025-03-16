#include "sequences.h"
#include "ajHeaders.h" 
#include "geometry/ssphere_ply.h" 
#include "geometry/head_ply.h"
#include "geometry/shead_ply.h" 
#include "geometry/woblevel_ply.h" 
#include "input.h" 

pvr_poly_hdr_t plasma2_header; 
pvr_poly_hdr_t pla2_level_header; 
pvr_poly_hdr_t ohead_header; 
pvr_poly_hdr_t thead_header; 

Camera pla2Cam; 
Quaternion pla2Q; 

static uint64_t animTimer; 

void render_plasma2()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.5f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	vec3f_t tv __attribute__((aligned(32)));
	static float rv = 0; 
	static float ry = 0; 
	static float rz = 0; 
	static float freq = 0;

	static float uvOffset = 0;
	 
	float a = 0.8f; 
	float ay = 0.4f; 
	float az = 0.6f; 
	pvr_prim(&plasma2_header, sizeof(plasma2_header));
	pvr_dr_init(&dr_state);

	rv+=0.1f; 
	ry+=0.06f;
	rz+=0.03f;
	
	if (rv > 6.283f) rv = 0; 
	if (ry > 6.283f) ry = 0; 
	if (rz > 6.283f) rz = 0; 
	
	uvOffset += 0.01f; 
	if (uvOffset >= 1.0f) uvOffset = 0; 
	
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
						  	
		tvert->u = ssphere_uvs[ssphere_faces[i].x].u + uvOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].x].v + uvOffset;
					
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

		tvert->u = ssphere_uvs[ssphere_faces[i].y].u + uvOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].y].v + uvOffset;
	
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

		tvert->u = ssphere_uvs[ssphere_faces[i].z].u + uvOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].z].v + uvOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	freq = 5;
	pvr_dr_finish();
}

void render_plasma2_thead()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 1.0f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	float rr = 1.0f, gg = 1.0f, bb = 1.0f, aa = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	
	static Vector normal;
	static Vector eye;
	
	static Vector light;// = Vector::Normalize(Vector(-1.0f, -0.5, -1.0f));
	static Vector l;
	
	//l = pla2Q * light; 
	//Vector half = Vector::Normalize(l + eye);
	static float intensity = 1.0f; 
	
	eye = Quaternion::Conjugate(pla2Cam.rotation) * Vector::Forward();
	light = Vector::Normalize(Vector(-1.0f, -0.5, -1.0f));
	l = Quaternion::Conjugate(pla2Q) * light; 
	pvr_prim(&thead_header, sizeof(thead_header));


	
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < HEAD_FACE_COUNT; ++i)
	{

	mat_trans_single3_nomod(head_verts[head_faces[i].x].x,
							head_verts[head_faces[i].x].y,
							head_verts[head_faces[i].x].z,
							v.x, v.y, v.z);

		if (v.x > 644.0f)continue; 
		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		normal.x = head_norms[head_faces[i].x].x;
		normal.y = head_norms[head_faces[i].x].y;
		normal.z = head_norms[head_faces[i].x].z;
		normal = pla2Q * normal; 
		intensity = MAX(ABS(Vector::Dot(normal, l)), 0);
		alpha = MIN(0.3f, aa * intensity);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
					
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);

		tvert->oargb = 0;
						  	
		tvert->u = head_uvs[head_faces[i].x].u; 
		tvert->v = head_uvs[head_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(head_verts[head_faces[i].y].x,
							head_verts[head_faces[i].y].y,
							head_verts[head_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);

		tvert->oargb = 0;								

		tvert->u = head_uvs[head_faces[i].y].u; 
		tvert->v = head_uvs[head_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(head_verts[head_faces[i].z].x,
							head_verts[head_faces[i].z].y,
							head_verts[head_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);

		tvert->oargb = 0;								

		tvert->u = head_uvs[head_faces[i].z].u; 
		tvert->v = head_uvs[head_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}


void render_plasma2_ohead()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	static float alpha = 1.0f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	static float rr = 1.0f, gg = 0.647f, bb = 0.0f;
	vec3f_t v __attribute__((aligned(32)));
	
	static Vector normal;
	static Vector eye;
	
	static Vector light;
	static Vector l;

	static float intensity = 1.0f; 

	light = Vector::Normalize(Vector(1.05f, 0.05f, -1.0f));
	l = pla2Q * light; 
	rr = 1.0f; 
	gg = 0.647f; 
	bb = 0.0f; 
	eye = Quaternion::Conjugate(pla2Cam.rotation) * -Vector::Right();
	pvr_prim(&ohead_header, sizeof(ohead_header));

	
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < SHEAD_FACE_COUNT; ++i)
	{
	
		//check for the flat shaded head instead for culling on this one; 

			normal.x = head_norms[head_faces[i].x].x;
			normal.y = head_norms[head_faces[i].x].y;
			normal.z = head_norms[head_faces[i].x].z;
		
		if (Vector::Dot(normal, eye) < 0.0f) continue; 
		
		normal.x = shead_norms[shead_faces[i].x].x;
		normal.y = shead_norms[shead_faces[i].x].y;
		normal.z = shead_norms[shead_faces[i].x].z;
		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
		
	mat_trans_single3_nomod(shead_verts[shead_faces[i].x].x,
							shead_verts[shead_faces[i].x].y,
							shead_verts[shead_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);

		tvert->oargb = 0;
						  	
		tvert->u = shead_uvs[shead_faces[i].x].u; 
		tvert->v = shead_uvs[shead_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(shead_verts[shead_faces[i].y].x,
							shead_verts[shead_faces[i].y].y,
							shead_verts[shead_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		
		normal.x = shead_norms[shead_faces[i].y].x;
		normal.y = shead_norms[shead_faces[i].y].y;
		normal.z = shead_norms[shead_faces[i].y].z;
		
		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);

		tvert->oargb = 0;								

		tvert->u = shead_uvs[shead_faces[i].y].u; 
		tvert->v = shead_uvs[shead_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(shead_verts[shead_faces[i].z].x,
							shead_verts[shead_faces[i].z].y,
							shead_verts[shead_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		
		normal.x = shead_norms[shead_faces[i].z].x;
		normal.y = shead_norms[shead_faces[i].z].y;
		normal.z = shead_norms[shead_faces[i].z].z;
		
		intensity = MAX(Vector::Dot(normal, l), 0);
		r = rr * intensity; 
		b = bb * intensity; 
		g = gg * intensity; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, r, g, b);

		tvert->oargb = 0;								

		tvert->u = shead_uvs[shead_faces[i].z].u; 
		tvert->v = shead_uvs[shead_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}

void render_plasma2_sphere()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 0.5f; 
	float r = 1.0f, g = 1.0f, b = 1.0f;
	vec3f_t v __attribute__((aligned(32)));
	vec3f_t tv __attribute__((aligned(32)));

	static float rv = 0; 
	static float ry = 0; 
	static float rz = 0; 
	static float freq = 0;

	static float uOffset = 0, vOffset = 0;
	float a = 0.08f; 
	
	Vector normal;
	Vector eye = Quaternion::Conjugate(pla2Cam.rotation) * -Vector::Right();
	
	pvr_prim(&plasma2_header, sizeof(plasma2_header));
	pvr_dr_init(&dr_state);
	
	static float xr = 0; 
	static float yr = 0; 
	static float zr = 0;
	
	xr+=0.1f; 
	yr+=0.06f;
	zr+=0.03f;
	if (xr > 6.283f) xr = 0; 
	if (yr > 6.283f) yr = 0; 
	if (zr > 6.283f) zr = 0; 

	rv = 1;
	
	freq = 40;
	
	if (rv > 6.283f) rv = 0; 
	if (ry > 6.283f) ry = 0; 
	if (rz > 6.283f) rz = 0; 

	float xx, yy, zz; 
	for(int i = 0; i < SSPHERE_FACE_COUNT; ++i)
	{
	
	normal.x = ssphere_norms[ssphere_faces[i].x].x;
	normal.y = ssphere_norms[ssphere_faces[i].x].y;
	normal.z = ssphere_norms[ssphere_faces[i].x].z;
	
	if (Vector::Dot(normal, eye) < 0.0f) continue; 

	r = 0.5f * (fsin((ssphere_norms[ssphere_faces[i].x].x)*freq - xr)+1.0f); 
	g = b = r; 
	
	xx = a * (fsin((ssphere_norms[ssphere_faces[i].x].x)*freq - xr)+1.0f);
	yy = a * (fsin((ssphere_norms[ssphere_faces[i].x].y)*freq - xr)+1.0f);
	zz = a * (fsin((ssphere_norms[ssphere_faces[i].x].z)*freq - xr)+1.0f);


	tv.x = ssphere_verts[ssphere_faces[i].x].x + ssphere_norms[ssphere_faces[i].x].x * xx; 
	tv.y = ssphere_verts[ssphere_faces[i].x].y + ssphere_norms[ssphere_faces[i].x].y * yy; 
	tv.z = ssphere_verts[ssphere_faces[i].x].z + ssphere_norms[ssphere_faces[i].x].z * zz; 
	 
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
						  	
		tvert->u = ssphere_uvs[ssphere_faces[i].x].u + uOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].x].v + vOffset;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	r = 0.5f * (fsin((ssphere_norms[ssphere_faces[i].y].x)*freq - xr)+1.0f); 
	g = b = r;
	
	xx = a * (fsin((ssphere_norms[ssphere_faces[i].y].x)*freq - xr)+1.0f);
	yy = a * (fsin((ssphere_norms[ssphere_faces[i].y].y)*freq - xr)+1.0f);
	zz = a * (fsin((ssphere_norms[ssphere_faces[i].y].z)*freq - xr)+1.0f);
	
	tv.x = ssphere_verts[ssphere_faces[i].y].x + ssphere_norms[ssphere_faces[i].y].x * xx; 
	tv.y = ssphere_verts[ssphere_faces[i].y].y + ssphere_norms[ssphere_faces[i].y].y * yy; 
	tv.z = ssphere_verts[ssphere_faces[i].y].z + ssphere_norms[ssphere_faces[i].y].z * zz; 
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

		tvert->u = ssphere_uvs[ssphere_faces[i].y].u + uOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].y].v + vOffset;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	r = 0.5f * (fsin((ssphere_norms[ssphere_faces[i].z].x)*freq - xr)+1.0f); 
	g = b = r;
	
	xx = a * (fsin((ssphere_norms[ssphere_faces[i].z].x)*freq - xr)+1.0f);
	yy = a * (fsin((ssphere_norms[ssphere_faces[i].z].y)*freq - xr)+1.0f);
	zz = a * (fsin((ssphere_norms[ssphere_faces[i].z].z)*freq - xr)+1.0f);
	 
	tv.x = ssphere_verts[ssphere_faces[i].z].x + ssphere_norms[ssphere_faces[i].z].x * xx; 
	tv.y = ssphere_verts[ssphere_faces[i].z].y + ssphere_norms[ssphere_faces[i].z].y * yy; 
	tv.z = ssphere_verts[ssphere_faces[i].z].z + ssphere_norms[ssphere_faces[i].z].z * zz; 
			
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

		tvert->u = ssphere_uvs[ssphere_faces[i].z].u + uOffset; 
		tvert->v = ssphere_uvs[ssphere_faces[i].z].v + vOffset;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	
	pvr_dr_finish();
}

void render_plasma2_level()
{
	pvr_vertex_t *tvert;
	pvr_dr_state_t dr_state;
	float alpha = 1.0f; 

	vec3f_t v __attribute__((aligned(32)));
	
	static float w = 0;
	
	pvr_prim(&pla2_level_header, sizeof(pla2_level_header));
	pvr_dr_init(&dr_state);
	
	for(int i = 0; i < WOBLEVEL_FACE_COUNT; ++i)
	{
	v.x = woblevel_verts[woblevel_faces[i].x].x;
	v.y = woblevel_verts[woblevel_faces[i].x].y;
	v.z = woblevel_verts[woblevel_faces[i].x].z;
	
	mat_trans_single3_nodivw(v.x,v.y,v.z,w); 

if (v.z < 7.0f) continue;
							
	mat_trans_single3_nomod(woblevel_verts[woblevel_faces[i].x].x,
							woblevel_verts[woblevel_faces[i].x].y,
							woblevel_verts[woblevel_faces[i].x].z,
							v.x, v.y, v.z);

		tvert = pvr_dr_target(dr_state);
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, 
						  woblevel_colors[woblevel_faces[i].x].r, 
						  woblevel_colors[woblevel_faces[i].x].g,
						  woblevel_colors[woblevel_faces[i].x].b);						  
		tvert->oargb = 0;
						  	
		tvert->u = woblevel_uvs[woblevel_faces[i].x].u; 
		tvert->v = woblevel_uvs[woblevel_faces[i].x].v;
					
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 

	mat_trans_single3_nomod(woblevel_verts[woblevel_faces[i].y].x,
							woblevel_verts[woblevel_faces[i].y].y,
							woblevel_verts[woblevel_faces[i].y].z,
							v.x, v.y, v.z);
										
		tvert = pvr_dr_target(dr_state);						
		tvert->flags = PVR_CMD_VERTEX; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, 
						  woblevel_colors[woblevel_faces[i].y].r, 
						  woblevel_colors[woblevel_faces[i].y].g,
						  woblevel_colors[woblevel_faces[i].y].b);
						  
		tvert->oargb = 0;								

		tvert->u = woblevel_uvs[woblevel_faces[i].y].u; 
		tvert->v = woblevel_uvs[woblevel_faces[i].y].v;
	
		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		pvr_dr_commit(tvert); 
		
	mat_trans_single3_nomod(woblevel_verts[woblevel_faces[i].z].x,
							woblevel_verts[woblevel_faces[i].z].y,
							woblevel_verts[woblevel_faces[i].z].z,
							v.x, v.y, v.z);
									
		tvert = pvr_dr_target(dr_state);			
		tvert->flags = PVR_CMD_VERTEX_EOL; 
		
		tvert->argb = PVR_PACK_COLOR(alpha, 
						  woblevel_colors[woblevel_faces[i].z].r, 
						  woblevel_colors[woblevel_faces[i].z].g,
						  woblevel_colors[woblevel_faces[i].z].b);
		tvert->oargb = 0;								

		tvert->u = woblevel_uvs[woblevel_faces[i].z].u; 
		tvert->v = woblevel_uvs[woblevel_faces[i].z].v;

		tvert->x = v.x;
		tvert->y = v.y;
		tvert->z = v.z;
		
		pvr_dr_commit(tvert); 
	}
	pvr_dr_finish();
}


void Plasma2Init()
{
	kos_texture_t* platex = load_png_texture("/rd/wave.png");
	CompileHeaderWithTexture(plasma2_header, TRTYPE, *platex);
	CompileHeader(pla2_level_header, OPTYPE);
	CompileHeader(ohead_header, OPTYPE);
	CompileHeader(thead_header, TRTYPE);
	
	animTimer = 8836 * 10 + timer_ms_gettime64();
}

void Plasma2Run()
{
		static Matrix plaT, plaS, plaR, plaT2, plaR2; 
       	static float r; 
       	static int i = 0; 
       	
		static float rx = 0, ry = 0, rz = 0;
		static float sf = 0, cf = 0, offset = 0; 
		static Vector lookTarget = Vector::Zero(); 
		static uint64_t snareTimer = timer_ms_gettime64();
		
		sf += 0.003f; 
		cf += 0.007f; 

		rx += 1.9f*fsin(sf); 
		ry += 0.78f*fsin(cf); 
		rz += 2.23f*fcos(cf);
		
		if (animTimer > timer_ms_gettime64())
		{
			if (snareTimer < timer_ms_gettime64())
			{

				snareTimer += 70*10;
				offset += 28.0f; 
			}
		}

		pla2Q = Quaternion::Euler(0, rz+offset, 45.0f * fsin(offset+rx*0.01f));
		pla2Cam.position = pla2Q * Vector::Forward() * 15; 
		pla2Cam.lookAt(lookTarget);
		pla2Cam.applyView();
		
       	if (gInput.buttonDown(BN_CROSS))
       		i ^= 1; 
       	
       	r +=0.01f;
       	pvr_wait_ready();
       	pvr_scene_begin();     	
       	pvr_list_begin(PVR_LIST_OP_POLY);
   			plaS.scale(1.0f, 1.0f, 1.0f);    		
       		mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&Camera::view.elem); 
		   	mat_apply(&plaT.elem); 
		   	mat_apply(&plaR.elem);
		   	mat_apply(&plaS.elem);

       		render_plasma2_level();      		
       		render_plasma2_ohead();
     	
       	pvr_list_finish();       	
       	pvr_list_begin(PVR_LIST_TR_POLY);

       		plaS.scale(2.6f, 2.6f, 2.6f);
          	mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&Camera::view.elem); 
		   	mat_apply(&plaT.elem); 
		   	mat_apply(&plaR.elem);
		   	mat_apply(&plaS.elem);
       		     	
       		render_plasma2_sphere();
   
   
   			plaS.scale(0.22f, 0.22f, 0.22f);
   			plaT2.translate(0.5f, 0.0f, 0.0f);

          	mat_identity();
		   	mat_apply(&proj.elem);
		   	mat_apply(&plaT2.elem); 
		   	plaR2.rotateZ(PI);
		   	mat_apply(&plaR2.elem);
		   	plaR2.rotateY(-((rz+offset)-90.0f)*PI/180.0f);
		   	mat_apply(&plaR2.elem);
		   	mat_apply(&plaS.elem);
		   	
   			render_plasma2_thead();

       	pvr_list_finish();
		pvr_scene_finish();
}
