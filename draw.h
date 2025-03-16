#ifndef __DRAW_H__
#define __DRAW_H__

#include "ply.h"
#include "matrix.h" 
#include <kos.h>

#define SW 640
#define SH 480

extern Matrix proj;

void draw_init(); 
void draw_close(); 

#endif/*__DRAW_H__*/
