#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#include "character.h"
#include "util.h"

struct character;
struct character_info;

typedef struct model{
	uint8_t *map;
	
	struct character_info **pc_info;
	int num_pc_info;
	struct character **squad;
	int num_pcs;
	
	int moveY;
	int moveX;
	int selection;
	int cur_pc;
	
	struct character *char_loc[GAME_HEIGHT * GAME_WIDTH];
}model_t;

int rotate_cur_pc();
int pc_move(struct character *c, int y, int x);

model_t *init_model();
void free_model(model_t*);

#endif