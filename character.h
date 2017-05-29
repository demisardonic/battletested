#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

typedef struct character{
	int x;
	int y;
	int speed;
	char c;
	int color;
	uint8_t *movement_map;
	int moves;
	
} character_t;

void update_valid_moves(uint8_t*, character_t*);
int move_character(uint8_t*, character_t*, int, int);

character_t *init_character();
void free_character(character_t*);

#endif