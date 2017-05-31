#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

#include "model.h"
struct model;

typedef struct character{
	int x;
	int y;
	int speed;
	char c;
	int color;
	uint8_t *movement_map;
	int turns;
} character_t;

void update_valid_moves(character_t**, uint8_t*, character_t*);
int move_character(struct model *, uint8_t*, character_t*, int, int);

character_t *init_character();
void free_character(character_t*);

#endif