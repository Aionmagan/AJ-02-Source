#include "pngloader.h"

kos_texture_t *load_png_texture(const char *filename, int nontwiddled) 
{
	kos_texture_t *texture;
	kos_img_t img;

	if (png_to_img(filename, PNG_FULL_ALPHA, &img) < 0) {
	return NULL;
	}

	texture = (kos_texture_t *)malloc(sizeof(kos_texture_t));
	if (!texture) {
	kos_img_free(&img, 0);
	return NULL;
	}

	texture->data = pvr_mem_malloc(img.byte_count);
	if (!texture->data) {
	free(texture);
	kos_img_free(&img, 0);
	return NULL;
	}

	texture->width = img.w;
	texture->height = img.h;

	if (nontwiddled)
	texture->fmt = PVR_TXRFMT_ARGB4444 | PVR_TXRFMT_NONTWIDDLED;
	else 
	texture->fmt = PVR_TXRFMT_ARGB4444;// | PVR_TXRFMT_NONTWIDDLED;

	pvr_txr_load_kimg(&img, texture->data, 0);
	kos_img_free(&img, 0);

	return texture;
}
