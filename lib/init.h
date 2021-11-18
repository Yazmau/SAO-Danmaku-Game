#ifndef _HEADER_

#define _HEADER_
#include "data.h"

#endif 


void allegro_init() {
	int i;
	
	exe(al_init(), "Fail to initialize allegro");
	exe(al_init_primitives_addon(), "Fail to initialize primitives add-on");
	exe(al_init_font_addon(), "Fail to initialize font add-on");
	exe(al_init_ttf_addon(), "Fail to initialize ttf add-on");
	exe(al_init_image_addon(), "Fail to initialize image add-on");
	exe(al_install_keyboard(), "Fail to install keyboard");
	exe(al_install_mouse(), "Fail to install mouse");
	
	exe(display = al_create_display(SCREEN_W + SCREEN_2_W, SCREEN_H), "Fail to create display");
	exe(event_queue = al_create_event_queue(), "Fail to creeate event queue");
	exe(update_timer = al_create_timer(1.0 / FPS), "Fail to create timer");
		
	memset(input_state.key, false, sizeof(input_state.key));
	const unsigned int mouth_buttons = al_get_mouse_num_buttons();
	game_log("There are %u supported mouse buttons", mouth_buttons);
	input_state.mouse = malloc((mouth_buttons + 1) * sizeof(bool));
	memset(input_state.mouse, false, sizeof(input_state.mouse));

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(update_timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	
	for(i=1;i<=50;i++)
		exe(font[i] = al_load_ttf_font("pirulen.ttf", i, 0), "Fail to load %s", "pirulen.ttf");
	
	al_start_timer(update_timer);
	game_log("Allegro5 initialized");
}

void game_init() {
	int i;
	
	// setup of plane
	for(i=1;i<=3;i++)
		plane_img[i] = load_bitmap_resized(string_merge(3, "image/plane_", int_to_string(i), ".png"), 70, 70);
	plane_img[4] = load_bitmap_resized(string_merge(3, "image/plane_", int_to_string(4), ".png"), 80, 104);
	plane_img[5] = load_bitmap_resized(string_merge(3, "image/plane_", int_to_string(5), ".png"), 100, 120);
	plane_img[6] = load_bitmap_resized(string_merge(3, "image/plane_", int_to_string(6), ".png"), 70, 70);
	plane_img[8] = load_bitmap_resized(string_merge(3, "image/plane_", int_to_string(8), ".png"), 782, 153);
	plane_img[9] = load_bitmap_resized(string_merge(3, "image/plane_", int_to_string(9), ".png"), 300, 91);
	
	// setup of player_plane
	player_plane.posx = 565;		player_plane.posy = 600;
	game_log("The posion of the plane is set in (%lf,%lf)", player_plane.posx, player_plane.posy);
	player_plane.speed = 3.0;
	game_log("The speed of the plane is set to %lf", player_plane.speed);
	player_plane.width = player_plane.height = 70;
	player_plane.kind = 1;
	player_plane.LV = 1;
	player_plane.ID = 1;
	player_plane.EXP = 0;
	player_plane.now_HP = 10;
	player_plane.HP_recover_cd = 60;
	player_plane.HP_recover_value = 2;
	player_plane.ammo = 100;
	player_plane.now_ammo = 100;
	player_plane.ammo_recover_cd = 60;
//		player_plane.EXP = 1000000;
	
	// setup of skill
	skill[0].cost = 20;
	skill[0].cd = 60;
	skill[0].now_cd = 0;
	skill[1].cost = 60;
	skill[1].cd = 600;
	skill[1].now_cd = 0;
	
	
	// setup of Boss
	Boss.posx = 560;		Boss.posy = 100;
	Boss.speed = 3.5;
	Boss.width = 80;	Boss.height = 104;
	Boss.kind = 4;
	Boss.LV = 35;
	Boss.ID = -1;
	Boss.EXP = Boss.LV*Boss.LV;
	Boss.HP = Boss.now_HP = 10000;
	Boss.attack = 5;
//		planeset_push(&enemy_plane_set, Boss);
	
	
	// setup of background
	for(i=0;i<=6;i++)
		background[i] = load_bitmap_resized(string_merge(3, "image/background_", int_to_string(i), ".png"), SCREEN_W + (i<5 ? SCREEN_2_W : 0), SCREEN_H);
	
	// setup of bullet_set
	for(i=1;i<=5;i++)
		bullet_img[i] = load_bitmap_resized(string_merge(3, "image/bullet_", int_to_string(i), ".png"), 17, 34);
	for(i=6;i<=7;i++)
		bullet_img[i] = load_bitmap_resized(string_merge(3, "image/bullet_", int_to_string(i), ".png"), 34, 34);
	bullet_img[8] = load_bitmap_resized(string_merge(3, "image/bullet_", int_to_string(8), ".png"), 50, 50);
	bullet_img[9] = load_bitmap_resized(string_merge(3, "image/bullet_", int_to_string(9), ".png"), 100, 120);
	for(i=10;i<=12;i+=2)
		bullet_img[i] = load_bitmap_resized(string_merge(3, "image/bullet_", int_to_string(i), ".png"), 148, 64);
	for(i=11;i<=13;i+=2)
		bullet_img[i] = load_bitmap_resized(string_merge(3, "image/bullet_", int_to_string(i), ".png"), 158, 36);
	bulletset_init(&bullet_set);
	
	
	game_log("Game initialized");
}

void init_keyboard() {
	int i;
	for(i=0;i<ALLEGRO_KEY_MAX;i++)
		input_state.key[i] = false;
}
