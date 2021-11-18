#ifndef _HEADER_

#define _HEADER_
#include "data.h"

#endif 

#define EPS 0.0001
#define DIAGONAL_FIX 0.707		// 1.0 / sqrt(2.0)


void event_loop() {
	ALLEGRO_EVENT event;
	
//	int redraw = 0;		// for optimization
	while(true) {
		al_wait_for_event(event_queue, &event);
		
		if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			game_log("Window has been closed");
			break;
		} 
		else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
			game_log("Key %d down", event.keyboard.keycode);
			input_state.key[event.keyboard.keycode] = true;
		}
		else if(event.type == ALLEGRO_EVENT_KEY_UP) {
			game_log("Key %d up", event.keyboard.keycode);
			input_state.key[event.keyboard.keycode] = false;
		}
		else if(event.type == ALLEGRO_EVENT_TIMER) {
			static time_cd = 0;
			game_update();
			game_draw();
			time_cd++;
			if(time_cd==FPS)
				timer++ , time_cd = 0;
		}
	}
	
	game_log("event loop finished");
}

void game_update() {
	static int update = 0;
	active_scene = update;
	if(active_scene==0) {
		if(input_state.key[ALLEGRO_KEY_ENTER])
			update = 3 , init_keyboard();
	}
	else if(active_scene==1) {
		
	}
	else if(active_scene==2) {
		
	}
	else if(active_scene==3) {
		if(input_state.key[ALLEGRO_KEY_J])
			update = 5 , timer = 0 , init_keyboard();
	}
	else if(active_scene==4) {
		
	}
	else if(active_scene==5) {
		bulletset_update(&bullet_set);
		if(update_player_plane()) {
			update = 2;
		}
		if(update_enemy_plane_1()) {
			update = 6;
			planeset_push(&enemy_plane_set, Boss);
		}
	}
	else if(active_scene==6) {
		bulletset_update(&bullet_set);
		if(update_player_plane()) {
			update = 2;
		}
		if(update_enemy_plane_2()) {
			update = 1;
		}
	}
}

void game_draw() {
	int i;
	al_clear_to_color(al_map_rgb(0, 0, 0));
	// draw background
	al_draw_bitmap(background[active_scene], 0, 0, 0);
	
	if(active_scene==0) {
		al_draw_text(font[30], al_map_rgb((timer+1)*119%256, (timer+1)*39%256, (timer+1)*71%256), (SCREEN_W+SCREEN_2_W)/2 , SCREEN_H/2,
	                 ALLEGRO_ALIGN_CENTER, "Press \"ENTER\" to continue");
	}
	else if(active_scene==1) {
		al_draw_bitmap(plane_img[8], (SCREEN_W+SCREEN_2_W)/2-346.0, 50, 0);
	}
	else if(active_scene==2) {
		al_draw_bitmap(plane_img[9], (SCREEN_W+SCREEN_2_W)/2-150.0, 10, 0);
	}
	else if(active_scene==3) {
		al_draw_text(font[50], al_map_rgb(128, 42, 42), (SCREEN_W+SCREEN_2_W)/2 , 50,
	                 ALLEGRO_ALIGN_CENTER, "How TO PLAY");
	    al_draw_text(font[40], al_map_rgb(255, 255, 255), 400 , 250,
	                 ALLEGRO_ALIGN_CENTER, "W A S D");
	    al_draw_text(font[25], al_map_rgb(255, 255, 255), 700 , 257.5,
	                 ALLEGRO_ALIGN_LEFT, "move up, left, down, right");
	    al_draw_text(font[40], al_map_rgb(255, 255, 255), 400 , 300,
	                 ALLEGRO_ALIGN_CENTER, "J");
	    al_draw_text(font[25], al_map_rgb(255, 255, 255), 700 , 307.5,
	                 ALLEGRO_ALIGN_LEFT, "basal attack");
	    al_draw_text(font[40], al_map_rgb(255, 255, 255), 400 , 350,
	                 ALLEGRO_ALIGN_CENTER, "K+W/A/S/D");
	    al_draw_text(font[25], al_map_rgb(255, 255, 255), 700 , 357.5,
	                 ALLEGRO_ALIGN_LEFT, "FISHING(Teleport)");
	    al_draw_text(font[40], al_map_rgb(255, 255, 255), 400 , 400,
	                 ALLEGRO_ALIGN_CENTER, "U");
	    al_draw_text(font[25], al_map_rgb(255, 255, 255), 700 , 407.5,
	                 ALLEGRO_ALIGN_LEFT, "STARBURST STREAM");
	                 
		al_draw_text(font[30], al_map_rgb((timer+1)*17%256, (timer+1)*61%256, (timer+1)*103%256), (SCREEN_W+SCREEN_2_W)/2 , SCREEN_H-100,
	                 ALLEGRO_ALIGN_CENTER, "Press \"J\" to start the game");
		al_draw_text(font[18], al_map_rgb(255, 255, 255), (SCREEN_W+SCREEN_2_W)/2 , SCREEN_H-63,
             ALLEGRO_ALIGN_CENTER, "remember to change your input method to English");
	}
	else if(active_scene==4) {
		
	}
	else if(active_scene==5||active_scene==6) {
		
		// draw bullets
		for(i=0;i<(int)bullet_set.size;i++)
			al_draw_bitmap(bullet_img[bullet_set.set[i].kind], bullet_set.set[i].posx, bullet_set.set[i].posy, 0);
		
		// draw player plane
		al_draw_bitmap(plane_img[player_plane.kind], player_plane.posx, player_plane.posy, 0);
		if(is_LV_change) {
			al_draw_text(font[14], al_map_rgb(127, 255, 212), player_plane.posx+(player_plane.width/2), player_plane.posy-13,
                 ALLEGRO_ALIGN_CENTER, "LEVEL UP");	
		}
		if(skill[0].used) {
			double dx = player_plane.posx - skill[0].pre_posx;
			double dy = player_plane.posy - skill[0].pre_posy;
			int number = 50;
			for(i=0;i<number;i++) {
				double posx = skill[0].pre_posx + dx*(double)i/(double)number; 
				double posy = skill[0].pre_posy + dy*(double)i/(double)number;
				al_draw_bitmap(plane_img[6], posx, posy, 0);
			}
			skill[0].used = false;
			skill[0].now_cd = skill[0].cd;
		}
	
		
		// draw information
		#define INTERVAL_DISTANCE 30
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W, 0,
	                 ALLEGRO_ALIGN_LEFT, "TIME");
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W + SCREEN_2_W - 3, 0,
	                 ALLEGRO_ALIGN_RIGHT, int_to_string(timer));
	    al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W, 1 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_LEFT, "SCORE");
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W + SCREEN_2_W - 3, 1 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_RIGHT, int_to_string(score));
	    al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W, 3 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_LEFT, "LEVEL");
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W + SCREEN_2_W, 3 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_RIGHT, int_to_string(player_plane.LV));
	    al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W, 4 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_LEFT, "HP");
		al_draw_rectangle(SCREEN_W + 45, 4 * INTERVAL_DISTANCE + 5, SCREEN_W + SCREEN_2_W - 2, 4 * INTERVAL_DISTANCE + 20, 
						al_map_rgb(34, 139, 34), 1);
		al_draw_filled_rectangle(SCREEN_W + 45 + (1.0 - (double)player_plane.now_HP/(double)player_plane.HP) * (SCREEN_2_W - 2 - 45), 4 * INTERVAL_DISTANCE + 5,
					 SCREEN_W + SCREEN_2_W - 2, 4 * INTERVAL_DISTANCE + 20,  al_map_rgb(34, 139, 34));
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W + SCREEN_2_W - 3 - 5, 4 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_RIGHT, string_merge(3, int_to_string(player_plane.now_HP), "/", int_to_string(player_plane.HP)));
	    al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W, 5 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_LEFT, "MP");
		al_draw_rectangle(SCREEN_W + 45, 5 * INTERVAL_DISTANCE + 5, SCREEN_W + SCREEN_2_W - 2, 5 * INTERVAL_DISTANCE + 20, 
						al_map_rgb(30, 144, 255), 1);
		al_draw_filled_rectangle(SCREEN_W + 45 + (1.0 - (double)player_plane.now_ammo/(double)player_plane.ammo) * (SCREEN_2_W - 2 - 45), 5 * INTERVAL_DISTANCE + 5,
					 SCREEN_W + SCREEN_2_W - 2, 5 * INTERVAL_DISTANCE + 20,  al_map_rgb(30, 144, 255));
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W + SCREEN_2_W - 3 - 5, 5 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_RIGHT, string_merge(3, int_to_string(player_plane.now_ammo), "/", int_to_string(player_plane.ammo)));	                 
	    al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W, 6 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_LEFT, "ATK");
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W + SCREEN_2_W, 6 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_RIGHT, int_to_string(player_plane.attack));

	    al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W, 8 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_LEFT, "K CD");
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W + SCREEN_2_W, 8 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_RIGHT, int_to_string(skill[0].now_cd));

	    al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W, 9 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_LEFT, "U CD");
		al_draw_text(font[20], al_map_rgb(255, 255, 255), SCREEN_W + SCREEN_2_W, 9 * INTERVAL_DISTANCE,
	                 ALLEGRO_ALIGN_RIGHT, int_to_string(skill[1].now_cd));
//		if(active_scene==5) {
			// draw enemy plane
			for(i=0;i<(int)enemy_plane_set.size;i++) {
				Plane now_plane = enemy_plane_set.set[i];
				al_draw_bitmap(plane_img[now_plane.kind], now_plane.posx, now_plane.posy, 0);
				al_draw_text(font[14], al_map_rgb(255, 0, 0), now_plane.posx+1, now_plane.posy-13,
		                 ALLEGRO_ALIGN_RIGHT, int_to_string(now_plane.LV));
				al_draw_rectangle(now_plane.posx+5, now_plane.posy-9, now_plane.posx+now_plane.width-1, now_plane.posy-1, 
							al_map_rgb(227, 23, 13), 1);
				al_draw_filled_rectangle(now_plane.posx+5 + (1.0 - (double)now_plane.now_HP/(double)now_plane.HP) * (now_plane.width-6), now_plane.posy-9,
						 now_plane.posx+now_plane.width-1, now_plane.posy-1,  al_map_rgb(227, 23, 13));
			}
//		}

	    	
	
	}
	al_flip_display();
	
}

bool update_player_plane() {	// update state of the player's plane
	if(player_plane.now_HP<=0.0)
		return true;
	int i;
	
	
	// state
	int pre_LV = player_plane.LV;
	player_plane.LV = (int)(sqrt(player_plane.EXP))+1;
	static int level_up_cd = 0;
	if(pre_LV!=player_plane.LV)		is_LV_change = true , level_up_cd = 120;
	if(level_up_cd>0)	level_up_cd--;
	if(level_up_cd==0)	is_LV_change = false;
	
	#define HP_CONSTANT 			10.0
	#define HP_COEFFICIENT			2.5
	player_plane.now_HP += ((double)(player_plane.LV-1) * HP_COEFFICIENT + HP_CONSTANT - player_plane.HP);
	player_plane.HP = (double)(player_plane.LV-1) * HP_COEFFICIENT + HP_CONSTANT;
	
	#define ATTACK_CONSTANT 		3.0
	#define ATTACK_COEFFICIENT		1.5
	player_plane.attack = (double)(player_plane.LV-1) * ATTACK_COEFFICIENT + ATTACK_CONSTANT;
	
	player_plane.now_ammo += ((player_plane.LV-1)*10 + 100 - player_plane.ammo);
	player_plane.ammo = (player_plane.LV-1)*10 + 100;
	player_plane.ammo_recover_value = (double)player_plane.ammo*0.02+player_plane.LV/10;

	static int hp_recover_cd = 0;
	if(hp_recover_cd==0) {
		player_plane.now_HP = min(player_plane.now_HP + player_plane.HP_recover_value, player_plane.HP);
		hp_recover_cd = player_plane.HP_recover_cd+player_plane.LV/10;
	}
	if(hp_recover_cd>0)	hp_recover_cd--;
	
	static int ammo_recover_cd = 0;
	if(ammo_recover_cd==0) {
		player_plane.now_ammo = min(player_plane.now_ammo + player_plane.ammo_recover_value, player_plane.ammo);
		ammo_recover_cd = player_plane.HP_recover_cd;
	}
	if(ammo_recover_cd>0)	ammo_recover_cd--;

	// player's plane shoots
	#define SHOOTING_CD 10
	#define BULLET_INIT_DISTANCE 5.0
	#define BULLET_SPEED_MULTIPLE 2.0
	static int shoot_cd = 0;
	if(!skill[1].used && shoot_cd==0 && input_state.key[ALLEGRO_KEY_J] && player_plane.now_ammo>=2) {
		Bullet new_bullet;
		new_bullet.posy = player_plane.posy + BULLET_INIT_DISTANCE;
		new_bullet.speedx = 0;
		new_bullet.speedy = player_plane.speed * BULLET_SPEED_MULTIPLE;
		new_bullet.width = 17;
		new_bullet.height = 34;
		new_bullet.damage = player_plane.attack;
		new_bullet.owner = 1;
		new_bullet.skill = -1;
		new_bullet.flag = -1;

		new_bullet.kind = 1;
		new_bullet.posx = player_plane.posx + (player_plane.width/3.0 - new_bullet.width/2.0);
		bulletset_push(&bullet_set, new_bullet);
		new_bullet.kind = 2;
		new_bullet.posx = player_plane.posx + (player_plane.width*2.0/3.0 - new_bullet.width/2.0);
		bulletset_push(&bullet_set, new_bullet);
		shoot_cd = SHOOTING_CD;
		
		
		player_plane.now_ammo -= 2;
	}
	if(shoot_cd>0)	shoot_cd--;


	// update position of player's plane
	double delta_x = (input_state.key[ALLEGRO_KEY_A] ? -1.0 : 0.0) + (input_state.key[ALLEGRO_KEY_D] ? 1.0 : 0.0);
	double delta_y = (input_state.key[ALLEGRO_KEY_W] ? -1.0 : 0.0) + (input_state.key[ALLEGRO_KEY_S] ? 1.0 : 0.0);
	if(fabs(delta_x-0.0)>EPS && fabs(delta_y-0.0)>EPS)		// move diagonallly
		delta_x *= DIAGONAL_FIX, delta_y *= DIAGONAL_FIX;
	
	if(skill[0].now_cd==0 && input_state.key[ALLEGRO_KEY_K] && player_plane.now_ammo>skill[0].cost && (fabs(delta_x-0.0)>EPS || fabs(delta_y-0.0)>EPS)) {	// use skill 1
		skill[0].used = true;
		skill[0].pre_posx = player_plane.posx;
		skill[0].pre_posy = player_plane.posy;
		player_plane.now_ammo -= skill[0].cost;
	}
	if(skill[0].now_cd>0)		skill[0].now_cd--;
	
	player_plane.posx += delta_x * player_plane.speed * (skill[0].used ? 50.0 : 1.0);
	player_plane.posx = min(max(player_plane.posx, 0), SCREEN_W-player_plane.width);
	player_plane.posy += delta_y * player_plane.speed * (skill[0].used ? 50.0 : 1.0);
	player_plane.posy = min(max(player_plane.posy, 0), SCREEN_H-player_plane.height);
	
	skill[1].timer++;
	if(skill[1].now_cd==0 && input_state.key[ALLEGRO_KEY_U] && player_plane.now_ammo>skill[1].cost) {
		skill[1].used = true;
		skill[1].timer = 0;
		player_plane.now_ammo -= skill[1].cost;
	}
	if(skill[1].used) {
		if(skill[1].timer==300) {
			skill[1].used = false;
			skill[1].now_cd = skill[1].cd;
			friend_plane_set.size = 0;
		}
		else {
			Bullet new_bullet;
			int tx = (random()%25+25)%25;
			int ty = (random()%25+25)%25;
			int x[] = {-80,13,13,-80};
			int y[] = {25,20,25,20};
			new_bullet.posx = player_plane.posx+(tx+x[skill[1].timer%4]);
			new_bullet.posy = player_plane.posy-(ty+y[skill[1].timer%4]);
			new_bullet.speedx = new_bullet.speedy = 0;
			int w[] = {148,158,148,158};
			int h[] = {64,36,64,36};
			new_bullet.width = w[skill[1].timer%4];
			new_bullet.height = h[skill[1].timer%4];
			
			new_bullet.damage = 1+player_plane.LV/3;
			new_bullet.owner = 1;
			new_bullet.skill = 1;
			new_bullet.flag = skill[1].timer%4;
	
			new_bullet.kind = 10+skill[1].timer%4;
			bulletset_push(&bullet_set, new_bullet);
	
			Plane new_plane;
			new_plane.posx = new_bullet.posx;
			new_plane.posy = new_bullet.posy;
			new_plane.speed = 0;
			new_plane.width = new_bullet.width;
			new_plane.height = new_bullet.height;
			new_plane.kind = 8+skill[1].timer%4;
			new_plane.ID = 1;
			new_plane.HP = 1;
			
			planeset_push(&friend_plane_set, new_plane);
		}
	}
	if(skill[1].now_cd>0)		skill[1].now_cd--;
	
	// destroy friendly plane
	int friend_plane_set_new_size = 0;
	for(i=0;i<(int)friend_plane_set.size;i++)
		if(friend_plane_set.set[i].now_HP > 0)
			friend_plane_set.set[friend_plane_set_new_size++] = friend_plane_set.set[i];
	friend_plane_set.size = friend_plane_set_new_size;
	
	return false;
}

Plane generate_enemy_plane(double posx, double posy, int kind, int LV) {
	Plane new_plane;
	new_plane.posx = posx;
	new_plane.posy = posy;
	new_plane.speed = 3.0;
	new_plane.width = new_plane.height = 70;
	new_plane.kind = kind;
	new_plane.ID = -1;
	new_plane.LV = LV;
	new_plane.HP = new_plane.now_HP = 30 + LV*11;
	new_plane.attack = 3 + LV;
	new_plane.EXP = LV;
	return new_plane;
}

bool update_enemy_plane_1() {
	int i;
	
	
	// generate enemy plane
	#define GENERATE_CD 180
	#define EXIST_LIMIT 8
	#define TOTAL_LIMIT 100
	static int generate_cd = 0;
	static int total_enemy_counter = 0;
	if(generate_cd==0) {
		int number = (random()%4+4)%4;
		while(number--) {
			if(total_enemy_counter < TOTAL_LIMIT && enemy_plane_set.size < EXIST_LIMIT) {
				double posx, posy;
				while(true) {
					posx = (random()%(SCREEN_W-70)+(SCREEN_W-70))%(SCREEN_W-70);
					posy = (random()%200+200)%200;
					Object tem = {posx, posy, 70, 70};
					bool is_hit = false;
					for(i=0;i<(int)enemy_plane_set.size;i++)
						if(object_intersect(tem, plane_to_object(&enemy_plane_set.set[i]))) {
							is_hit = true;
							break;
						}
					if(!is_hit)		break;
				}
				Plane new_plane = generate_enemy_plane(posx, posy, 3, total_enemy_counter/10+1);
				planeset_push(&enemy_plane_set, new_plane);
				game_log("Generate a enemy at (%lf,%lf) with speed %lf", new_plane.posx, new_plane.posy, new_plane.speed);
				game_log("The number of enemys is %u", enemy_plane_set.size);
	
				total_enemy_counter++;
				generate_cd = GENERATE_CD;
			}
		}
	}
	if(generate_cd>0)	generate_cd--;

	// destroy enemy plane
	int enemy_plane_set_new_size = 0;
	for(i=0;i<(int)enemy_plane_set.size;i++)
		if(enemy_plane_set.set[i].now_HP > 0)
			enemy_plane_set.set[enemy_plane_set_new_size++] = enemy_plane_set.set[i];
		else {
			player_plane.EXP += enemy_plane_set.set[i].EXP*1.5+1;
			player_plane.now_ammo = min(player_plane.now_ammo + (double)player_plane.ammo*0.04, player_plane.ammo);
			score += 1000 - timer + enemy_plane_set.set[i].LV*10;
		}
	enemy_plane_set.size = enemy_plane_set_new_size;
	
	// enemy plane shoot
	for(i=0;i<(int)enemy_plane_set.size;i++)
		if(possibility(2)) {
			Plane now_plane = enemy_plane_set.set[i];
			Bullet new_bullet;
			new_bullet.width = 17;
			new_bullet.height = 34;
			new_bullet.posy = now_plane.posy+now_plane.height-BULLET_INIT_DISTANCE;
			new_bullet.posx = now_plane.posx + (now_plane.width/2.0 - new_bullet.width/2.0);
			new_bullet.speedx = now_plane.speed * BULLET_SPEED_MULTIPLE * (now_plane.LV >= 3 && possibility(20+min(30,(now_plane.LV-3)*2)) ? (possibility(50) ? -1 : 1) : 0);
			new_bullet.speedy = now_plane.speed * BULLET_SPEED_MULTIPLE;
			if(new_bullet.speedx!=0) {
				new_bullet.speedx *= DIAGONAL_FIX;
				new_bullet.speedx *= DIAGONAL_FIX;
			}
			new_bullet.damage = now_plane.attack;
			new_bullet.owner = -1;

			new_bullet.kind = 3;
			if(new_bullet.speedx > 0)
				new_bullet.kind = 4;
			else if(new_bullet.speedx <0)
				new_bullet.kind = 5;
			bulletset_push(&bullet_set, new_bullet);
		}
		
	// update position of enemy planes
	for(i=0;i<(int)enemy_plane_set.size;i++) {
		Plane* ptr = &enemy_plane_set.set[i];
		double delta_x = (possibility(33) ? 0 : possibility(50) ? -1 : 1);
		double delta_y = (possibility(33) ? 0 : possibility(50) ? -1 : 1);
		if(fabs(delta_x-0.0)>EPS && fabs(delta_y-0.0)>EPS)		// move diagonallly
			delta_x *= DIAGONAL_FIX, delta_y *= DIAGONAL_FIX;
		
		if(out_x_bound(ptr->posx + delta_x * ptr->speed, ptr->width)==-1) 	ptr->posx += delta_x * ptr->speed;
		if(out_y_bound(ptr->posy + delta_y * ptr->speed, ptr->height)==-1)	ptr->posy += delta_y * ptr->speed;
	
	}
	
	if(total_enemy_counter==TOTAL_LIMIT && enemy_plane_set.size==0) {
		return true;
	}
	return false;
}

bool update_enemy_plane_2() {
	int i;
	
	static int boss_timer = 0;
	static int boss_timer2 = 0;
	
	Plane* Boss = &enemy_plane_set.set[0];
	
	if(Boss->now_HP < 0) {
		return true;
	}

	// skill
	if(boss_timer!=0 && boss_timer%(int)(240.0-60.0*(1.0-(double)Boss->now_HP/(double)Boss->HP))==0) {		// skill 1
		Bullet new_bullet;
		new_bullet.width = new_bullet.height = 34;
		new_bullet.posx = Boss->posx + Boss->width/2.0 - 17.0;
		new_bullet.posy = Boss->posy + Boss->height/2.0 - 17.0;
		new_bullet.damage = Boss->attack + 1.5*(1.0-(double)Boss->now_HP/(double)Boss->HP);
		new_bullet.owner = -2;
		new_bullet.kind = 7;
		int balls_number = 36+72.0*(1.0-(double)Boss->now_HP/(double)Boss->HP);		// 36~108 balls according to hp of the boss
		for(i=0;i<balls_number;i++) {
			new_bullet.speedx = Boss->speed * 0.7 * sin(2.0*PI*(double)(i)/(double)balls_number);
			new_bullet.speedy = Boss->speed * 0.7 * cos(2.0*PI*(double)(i)/(double)balls_number);
			bulletset_push(&bullet_set, new_bullet);
		}
	}
	if(boss_timer!=0 && boss_timer%(int)(300.0-120.0*(1.0-(double)Boss->now_HP/(double)Boss->HP))==0) {		// skill 2
		Bullet new_bullet;
		new_bullet.width = new_bullet.height = 50;
		new_bullet.posx = Boss->posx + Boss->width/2.0 - 17.0;
		new_bullet.posy = Boss->posy + Boss->height/2.0 - 17.0;
		new_bullet.damage = Boss->attack*2.5;
		new_bullet.owner = -2;
		new_bullet.kind = 8;
		int balls_number = 16+12.0*(1.0-(double)Boss->now_HP/(double)Boss->HP);		// 16~28 balls according to hp of the boss
		for(i=0;i<balls_number;i++) {
			new_bullet.speedx = Boss->speed * 0.7 * sin(2.0*PI*(double)(i)/(double)balls_number);
			new_bullet.speedy = Boss->speed * 0.7 * cos(2.0*PI*(double)(i)/(double)balls_number);
			bulletset_push(&bullet_set, new_bullet);
		}
	}
	if(boss_timer!=0 && boss_timer%300==0) {		// skill 3
		if(enemy_plane_set.size==1 || (enemy_plane_set.size==2 && enemy_plane_set.set[1].now_HP!=Boss->HP*0.1)) {		// destroy the previous shield
			if(enemy_plane_set.size==2 && enemy_plane_set.set[1].now_HP!=Boss->HP*0.1)
				enemy_plane_set.size--;
			
			Plane shield;
			shield.posx = Boss->posx+Boss->width/2-50;
			shield.posy = Boss->posy+Boss->height+30;
			shield.width = 100;
			shield.height = 120;
			shield.LV = 30;
			shield.HP = shield.now_HP = Boss->HP*0.1;
			shield.kind = 5;
			planeset_push(&enemy_plane_set, shield);
			boss_timer2 = 0;
		}
	}
	if(boss_timer!=0 && boss_timer2!=0 && boss_timer2%(120+(random()%600+600)%600)==0 && enemy_plane_set.size==2) {	// skill 4
		Boss->posx = player_plane.posx+player_plane.width/2-Boss->width/2;
		enemy_plane_set.size--;
		Bullet new_bullet;
		new_bullet.width = 100;
		new_bullet.height = 120;
		new_bullet.posx = Boss->posx+Boss->width/2-50;
		new_bullet.posy = Boss->posy+Boss->height+30;
		new_bullet.damage = Boss->attack + 1.5*(1.0-(double)Boss->now_HP/(double)Boss->HP);
		new_bullet.owner = -2;
		new_bullet.kind = 9;
		new_bullet.speedx = 0;
		new_bullet.speedy = Boss->speed * 1.7;
		for(i=0;i<6;i++) {
			bulletset_push(&bullet_set, new_bullet);
			new_bullet.posy += 8;
		}
	}
	
	if(enemy_plane_set.size==2 && enemy_plane_set.set[1].now_HP<=0)
		enemy_plane_set.size--;
	
	static double vx = 0 , vy = 0;
	if(boss_timer%180==0) {
		int target_x = (random()%(SCREEN_W-(int)Boss->width)+(SCREEN_W-(int)Boss->width))%(SCREEN_W-(int)Boss->width);
		int target_y = (random()%100+100)%100;
		vx = ((double)target_x-vx)/480.0;
		vy = ((double)target_y-vy)/480.0;
	}
	
	// update the position of Boss
	Boss->posx += vx;;
	Boss->posx = min(max(Boss->posx, 0), SCREEN_W-Boss->width);
	Boss->posy += vy;
	Boss->posy = min(max(Boss->posy, 0), SCREEN_H-Boss->height);
		
	// update the position of shield	
	if(enemy_plane_set.size==2) {
		enemy_plane_set.set[1].posx = Boss->posx+Boss->width/2-50;
		enemy_plane_set.set[1].posy = Boss->posy+Boss->height+30;
	}
	boss_timer++;
	boss_timer2++;
	return false;
}
