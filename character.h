#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

typedef struct character{
	int x;
	int y;
	char c;
	int color;
	
} character_t;

int move_character(uint8_t *map, character_t *character, int x, int y);

#endif