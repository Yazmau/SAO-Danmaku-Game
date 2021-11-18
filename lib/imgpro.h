#ifndef _HEADER_

#define _HEADER_
#include "data.h"

#endif 

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
	ALLEGRO_BITMAP* loaded_bmp;
	exe(loaded_bmp = al_load_bitmap(filename), "fail to load image: %s", filename);
	
	ALLEGRO_BITMAP *resized_bmp;
	exe(resized_bmp = al_create_bitmap(w, h), "fail to create bitmap when creating resized image: %s", filename);
	ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

	al_set_target_bitmap(resized_bmp);
	al_draw_scaled_bitmap(loaded_bmp, 0, 0,
		al_get_bitmap_width(loaded_bmp),
		al_get_bitmap_height(loaded_bmp),
		0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	game_log("resized image: %s", filename);

	return resized_bmp;
}
