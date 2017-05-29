#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#include "character.h"

typedef struct model model_t;
struct model{
	uint8_t *map;
	character_t *player;
};

int pc_move_left();
int pc_move_right();
int pc_move_up();
int pc_move_down();

model_t *init_model();
void free_model(model_t*);

#endif