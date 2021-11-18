#include <stdio.h>
#include <string.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define _HEADER_ 

const unsigned int SCREEN_H = 900;
const unsigned int SCREEN_W = 1200;

const unsigned int SCREEN_2_W = 300;

const unsigned int FPS = 60;

const double PI = acos(-1.0);

int active_scene = 1;	// 0  : menu
						// 1  : win
						// 2  : lose
						// 3  : how to play
						// 4  : setting
						// 5  : battle stage 1
						// 6  : battle stage 1 BOSS
						

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_FONT* font[51];
ALLEGRO_TIMER* update_timer;

int timer = 0;
int score = 0;

struct InputState {
	bool key[ALLEGRO_KEY_MAX];
	bool *mouse;
}input_state;



ALLEGRO_BITMAP* background[10];
ALLEGRO_BITMAP* plane_img[10];
ALLEGRO_BITMAP* bullet_img[15];
ALLEGRO_BITMAP* lose_img[1];



typedef struct {
	double posx, posy;
	double speed;
	double width, height;
	int kind;
	
	int ID, LV, EXP;
	int HP, now_HP;
	int attack;
	
	int HP_recover_cd;
	int HP_recover_value;
	
	int ammo, now_ammo;
	int ammo_recover_cd;
	int ammo_recover_value;
}Plane;
bool is_LV_change = false;
Plane player_plane;
struct Skill{
	bool used;
	double pre_posx, pre_posy;
	int timer;
	int cd, now_cd;
	int cost;
}skill[10];
Plane Boss;

struct PlaneSet {
	#define CAPICITY_LIMIT 100005		// I think this number is big enough
	Plane set[CAPICITY_LIMIT];
	unsigned int size;
}friend_plane_set, enemy_plane_set;


typedef struct {
	double posx, posy;
	double speedx, speedy;
//	double direction;
	double width, height;
	int damage;
	int owner;		// 0 : NULL , Z+ : friend , Z- : enemy
	int kind;
	int skill;
	int flag;
}Bullet;

struct BulletSet {
	#define CAPICITY_LIMIT 100005		// I think this number is big enough
	Bullet set[CAPICITY_LIMIT];
	unsigned int size;
}bullet_set;

typedef struct {
	int posx, posy;
	int width, height;
}Object;

// debug.h
void game_vlog(char* format, va_list arg);
void game_log(char* format, ...);
void exe(bool is_succesful_to_execution, char *error_message, ...);

// function.h
double min(double,double);
double max(double,double);
int random();
bool possibility(double threshold);
char* int_to_string(int val);
char* string_merge(int size, ...);
void planeset_init(struct PlaneSet* ptr);
void planeset_push(struct PlaneSet* ptr,Plane now);		// add a enemy plane into the set
void bulletset_init(struct BulletSet* ptr);
void bulletset_push(struct BulletSet* ptr,Bullet now);		// add a bullet into the set
void bulletset_update(struct BulletSet* ptr);		// update the state of all the bullets
int out_x_bound(double posx,double width);		// -1 : no , 0 : intersect , 1 : out completely
int out_y_bound(double posy,double height);
Object plane_to_object(const Plane* ptr);
Object bullet_to_object(const Bullet* ptr);
bool segment_intersect(double a1,double a2,double b1,double b2);		// a1 < a2 , b1 < b2
bool object_intersect(Object a, Object b);

// imgpro.h
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);

// init.h
void allegro_init();
void game_init();
void init_keyboard();

// game.h
void event_loop();
void game_update();
void game_draw();
bool update_player_plane();
Plane generate_enemy_plane(double posx, double posy, int kind, int LV);
bool update_enemy_plane_1();
bool update_enemy_plane_2();
