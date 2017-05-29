#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "character.h"
#include "util.h"

static void update_valid_moves_rec(uint8_t *map, uint8_t *done, int dist, int y, int x){
	done[yx_to_index(y, x)] = dist;
	if(dist > 0){
		if(y-1 >= 0 && dist > done[yx_to_index(y-1, x)] && !map[yx_to_index(y-1, x)]){
			update_valid_moves_rec(map, done, dist-1, y-1, x);
		}
		if(y+1 < GAME_HEIGHT && dist > done[yx_to_index(y+1, x)] && !map[yx_to_index(y+1, x)]){
			update_valid_moves_rec(map, done, dist-1, y+1, x);
		}
		if(x-1 >= 0 && dist > done[yx_to_index(y, x-1)] && !map[yx_to_index(y, x-1)]){
			update_valid_moves_rec(map, done, dist-1, y, x-1);
		}
		if(x+1 < GAME_WIDTH && dist > done[yx_to_index(y, x+1)] && !map[yx_to_index(y, x+1)]){
			update_valid_moves_rec(map, done, dist-1, y, x+1);
		}
	}
}

void update_valid_moves(uint8_t *map, character_t *character){
	memset(character->movement_map, 0, sizeof(uint8_t) * GAME_HEIGHT * GAME_WIDTH);
	update_valid_moves_rec(map, character->movement_map, character->speed, character->y, character->x);
}

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
	update_valid_moves(map, character);
	return 0;
}

character_t *init_character(uint8_t *map){
	character_t *ch = malloc(sizeof(character_t));
	ch->movement_map = malloc(sizeof(uint8_t) * GAME_HEIGHT * GAME_WIDTH);
	memset(ch->movement_map, 0, sizeof(uint8_t) * GAME_HEIGHT * GAME_WIDTH);
	ch->x = 0;
	ch->y = 0;
	ch->speed = 3;
	ch->color = COLOR_DEFAULT;
	ch->c = 'a';
	return ch;
}

void free_character(character_t* c){
	free(c->movement_map);
	free(c);
}