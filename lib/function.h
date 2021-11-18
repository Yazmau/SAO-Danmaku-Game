#ifndef _HEADER_

#define _HEADER_
#include "data.h"

#endif 


inline double min(double a,double b) {
	return a < b ? a : b;
}

inline double max(double a,double b) {
	return a > b ? a : b;
}

inline int random() {
	static int seed = 20190115;
    return seed += (seed<<16) + 0x1db3d743; 
}
inline bool possibility(double threshold) {
	int now = (random()%10000+10000)%10000;
	return now<=(int)(threshold*100.0);
}

char* int_to_string(int val) {
	char* ret;
	ret = (char* )malloc(25*sizeof(char));
	int size = 0;
	if(val==0) {
		ret[size++] = '0';
	}
	else {
		while(val) {
			ret[size++] = (val%10)+'0';
			val /= 10;
		}
	}	
	ret[size] = '\0';
	int i,j;
	for(i=0,j=size-1;i<j;i++,j--) {
		char tem = ret[j];
		ret[j] = ret[i];
		ret[i] = tem;
	}
	return ret;
}

char* string_merge(int size, ...) {
	char* ret;
	ret = (char* )malloc(100*sizeof(char));
	int ret_size = 0;
	va_list arg;
	va_start(arg, size);
	while(size--) {
		char* tem = va_arg(arg, char*);
		int i;
		for(i=0;i<strlen(tem);i++)
			ret[ret_size++] = tem[i];
	}
	ret[ret_size] = '\0';
	va_end(arg);
	return ret;
}

void planeset_init(struct PlaneSet* ptr) {
	ptr->size = 0;
}

void planeset_push(struct PlaneSet* ptr,Plane now) {		// add a enemy plane into the set
	ptr->set[ptr->size++] = now;
}


void bulletset_init(struct BulletSet* ptr) {
	ptr->size = 0;
}

void bulletset_push(struct BulletSet* ptr,Bullet now) {		// add a bullet into the set
	ptr->set[ptr->size++] = now;
}

void bulletset_update(struct BulletSet* ptr) {		// update the state of all the bullets
	int i,j;
	
	// update the position of all the bullets
	for(i=0;i<ptr->size;i++) {
		ptr->set[i].posx -= (double)ptr->set[i].owner*ptr->set[i].speedx;
		ptr->set[i].posy -= (double)ptr->set[i].owner*ptr->set[i].speedy;
	}
	
	// destroy the bullet
	int bullet_set_new_size = 0;
	for(i=0;i<ptr->size;i++) {
		bool is_hit = false;
		if(ptr->set[i].owner==1) {
			if(ptr->set[i].skill!=-1 && ptr->set[i].flag!=skill[1].timer%4)
				is_hit = true;
			else
				for(j=0;j<(int)enemy_plane_set.size;j++)
					if(object_intersect(plane_to_object(&enemy_plane_set.set[j]),bullet_to_object(&ptr->set[i]))) {
						enemy_plane_set.set[j].now_HP -= ptr->set[i].damage;
						is_hit = true;
					}
		}
		else {
			for(j=0;j<(int)friend_plane_set.size;j++)
				if(object_intersect(plane_to_object(&friend_plane_set.set[j]),bullet_to_object(&ptr->set[i]))) {
					friend_plane_set.set[j].now_HP -= ptr->set[i].damage;
					player_plane.now_ammo = min(player_plane.now_ammo+1, player_plane.ammo);
					is_hit = true;
				}
			if(!is_hit && object_intersect(plane_to_object(&player_plane), bullet_to_object(&ptr->set[i]))) {
				player_plane.now_HP = max(player_plane.now_HP-ptr->set[i].damage, 0.0);
				is_hit = true;
			}
		}
		if(!is_hit && out_x_bound(ptr->set[i].posx, ptr->set[i].width)!=1 && out_y_bound(ptr->set[i].posy, ptr->set[i].height)!=1)
			ptr->set[bullet_set_new_size++] = ptr->set[i];
	}
	ptr->size = (unsigned int)bullet_set_new_size;
	
}



inline int out_x_bound(double posx,double width) {		// -1 : no , 0 : intersect , 1 : out completely
	int ret = -1;
	if(posx < 0.0 || posx+width > SCREEN_W)
		ret = 0;
	if(posx+width < 0.0 || posx > SCREEN_W)
		ret = 1;
	return ret;
}

inline int out_y_bound(double posy,double height) {
	int ret = -1;
	if(posy < 0.0 || posy+height > SCREEN_H)
		ret = 0;
	if(posy+height < 0.0 || posy > SCREEN_H)
		ret = 1;
	return ret;
}

Object plane_to_object(const Plane* ptr) {
	Object ret;
	ret.posx = ptr->posx;
	ret.posy = ptr->posy;
	ret.width = ptr->width;
	ret.height = ptr->height;
	return ret;
}

Object bullet_to_object(const Bullet* ptr) {
	Object ret;
	ret.posx = ptr->posx;
	ret.posy = ptr->posy;
	ret.width = ptr->width;
	ret.height = ptr->height;
	return ret;
}

inline bool segment_intersect(double a1,double a2,double b1,double b2) {		// a1 < a2 , b1 < b2
	return max(a1,b1) <= min(a2,b2);
}
inline bool object_intersect(Object a, Object b) {
	return segment_intersect(a.posx, a.posx+a.width, b.posx, b.posx+b.width) && 
					segment_intersect(a.posy, a.posy+a.height, b.posy, b.posy+b.height);
}

