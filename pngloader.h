#ifndef __PNGLOADER_H__
#define __PNGLOADER_H__

#include <png/png.h> 
#include <stdlib.h>

typedef struct {
    pvr_ptr_t data;
    int width, height;
    uint32 fmt;
} kos_texture_t;


kos_texture_t *load_png_texture(const char *filename, int nontwiddled = 0);

#endif
