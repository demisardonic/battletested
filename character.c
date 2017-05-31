#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "character.h"
#include "model.h"
#include "util.h"

static void update_valid_moves_rec(character_t **char_loc, uint8_t *map, uint8_t *done, int dist, int y, int x){
	done[yx_to_index(y, x)] = dist;
	if(dist > 0){
		if(y-1 >= 0 && dist >= done[yx_to_index(y-1, x)] && !map[yx_to_index(y-1, x)] && !char_loc[yx_to_index(y-1, x)]){
			update_valid_moves_rec(char_loc, map, done, dist-1, y-1, x);
		}
		if(y+1 < GAME_HEIGHT && dist >= done[yx_to_index(y+1, x)] && !map[yx_to_index(y+1, x)] && !char_loc[yx_to_index(y+1, x)]){
			update_valid_moves_rec(char_loc, map, done, dist-1, y+1, x);
		}
		if(x-1 >= 0 && dist >= done[yx_to_index(y, x-1)] && !map[yx_to_index(y, x-1)] && !char_loc[yx_to_index(y, x-1)]){
			update_valid_moves_rec(char_loc, map, done, dist-1, y, x-1);
		}
		if(x+1 < GAME_WIDTH && dist >= done[yx_to_index(y, x+1)] && !map[yx_to_index(y, x+1)] && !char_loc[yx_to_index(y, x+1)]){
			update_valid_moves_rec(char_loc, map, done, dist-1, y, x+1);
		}
	}
}

void update_valid_moves(character_t **char_loc, uint8_t *map, character_t *character){
	int distance = (character->speed * character->turns) + 1;
	memset(character->movement_map, 0, sizeof(uint8_t) * GAME_HEIGHT * GAME_WIDTH);
	update_valid_moves_rec(char_loc, map, character->movement_map, distance, character->y, character->x);
}

int move_character(model_t *model, character_t *character, int y, int x){
	if(!character){
		return 0;
	}
	
	if(x < 0 || y < 0 || x >= GAME_WIDTH || y >= GAME_HEIGHT){
		return 0;
	}
	
	if(model->map[yx_to_index(y, x)] != 0){
		return 0;
	}
	
	int cost;
	if(character->y == y && character->x == x){
		cost = 0;
		return cost;
	} 
	
	cost = character->turns - ((character->movement_map[yx_to_index(y, x)] - 1) / character->speed);
		 
	character->turns -= cost;
	model->char_loc[yx_to_index(character->y, character->x)] = NULL;
	character->y = y;
	character->x = x;
	model->char_loc[yx_to_index(character->y, character->x)] = character;
	int i;
	for(i = 0; i < model->num_pcs; i++){
		update_valid_moves(model->char_loc, model->map, model->pcs[i]);
	}
	return cost;
}

character_t *init_character(){
	character_t *ch = (character_t *)malloc(sizeof(character_t));
	ch->movement_map = (uint8_t *)malloc(sizeof(uint8_t) * GAME_HEIGHT * GAME_WIDTH);
	memset(ch->movement_map, 0, sizeof(uint8_t) * GAME_HEIGHT * GAME_WIDTH);
	ch->x = rand() % GAME_WIDTH;
	ch->y = rand() % GAME_HEIGHT;
	ch->speed = 5;
	ch->color = COLOR_DEFAULT;
	ch->c = 'a';
	ch->turns = 2;
	return ch;
}

void free_character(character_t* c){
	free(c->movement_map);
	free(c);
}