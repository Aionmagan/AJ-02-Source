#include "common.h" 

float lerpf(float a, float b, float d)
{
	return (a + d * (b - a)); 
}

double fastPow(double base, int exponent) {
    double result = 1.0;

    while (exponent != 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }

    return result;
}

void CompileHeader(pvr_poly_hdr_t& ply_header, PlyHeaderType type, int flag)
{
	pvr_poly_cxt_t tmp; 	
	switch(type)
	{
		case OPTYPE:
		default:
			pvr_poly_cxt_col(&tmp, PVR_LIST_OP_POLY);
			break;
		case TRTYPE:
			pvr_poly_cxt_col(&tmp, PVR_LIST_TR_POLY);
			break;
	}
    tmp.gen.culling = flag;
	tmp.gen.shading = PVR_SHADE_GOURAUD;
	tmp.depth.comparison = PVR_DEPTHCMP_GEQUAL; 
	tmp.depth.write = PVR_DEPTHWRITE_ENABLE; 
    pvr_poly_compile(&ply_header, &tmp);
}

void CompileHeaderWithTexture(pvr_poly_hdr_t& ply_header, PlyHeaderType type, kos_texture_t & kt, int flag)
{
	pvr_poly_cxt_t tmp;
	switch(type)
	{
		case OPTYPE:
			pvr_poly_cxt_txr(&tmp, PVR_LIST_OP_POLY, PVR_TXRFMT_ARGB4444,
                     kt.width, kt.height,
                     kt.data, PVR_FILTER_BILINEAR);
			break;
		case TRTYPE:
			pvr_poly_cxt_txr(&tmp, PVR_LIST_TR_POLY, PVR_TXRFMT_ARGB4444,
                     kt.width, kt.height,
                     kt.data, PVR_FILTER_BILINEAR);
			break;
	}

    tmp.gen.culling = flag; 
	tmp.gen.shading = PVR_SHADE_GOURAUD;
	tmp.depth.comparison = PVR_DEPTHCMP_GEQUAL; 
	tmp.depth.write = PVR_DEPTHWRITE_ENABLE; 
    pvr_poly_compile(&ply_header, &tmp);
}
