#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

#include "model.h"

struct model;

typedef struct character_info{
	char* f_name;
	char* l_name;
	uint8_t stats[7];
	uint8_t in_squad;
} character_info_t;

typedef struct character{
	int x;
	int y;
	int speed;
	char c;
	int color;
	uint8_t *movement_map;
	int turns;
	character_info_t *info;
} character_t;

void update_valid_moves(character_t**, uint8_t*, character_t*);
int move_character(struct model *, character_t*, int, int);

void free_character_info(character_info_t*);

character_t *init_character(character_info_t *info);
void free_character(character_t*);

#endif