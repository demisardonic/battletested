#include <stdint.h>

#include "character.h"
#include "util.h"

//Returns the cost of the move
int move_character(uint8_t *map, character_t *character, int y, int x){
	if(!character){
		return 1;
	}
	
	if(x < 0 || y < 0 || x >= GAME_WIDTH || y >= GAME_HEIGHT){
		return 1;
	}
	
	if(map[yx_to_index(y, x)] != 0){
		return 1;
	}
	
	character->y = y;
	character->x = x;
	return 0;
}