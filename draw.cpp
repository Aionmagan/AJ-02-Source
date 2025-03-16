#include "draw.h" 
#include <cmath>
#include <dc/fmath.h>

pvr_init_params_t params = {
    /* Enable opaque and translucent polygons with size 16 */
    { PVR_BINSIZE_32, PVR_BINSIZE_0, PVR_BINSIZE_32, PVR_BINSIZE_0, PVR_BINSIZE_32 },

    /* Vertex buffer size 512K */
    1<<20, 0, 0, 0, 3
};

Matrix proj;

void draw_init()
{
	pvr_init(&params); 
	
	proj.perspectiveProj(45.0f, ((float)SW/(float)SH), 0.001f, 1000.0f); 
}

void draw_close()
{
	pvr_shutdown();
}

