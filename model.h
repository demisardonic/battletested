#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#include "character.h"

typedef struct model{
	uint8_t *map;
	int selY;
	int selX;
	character_t **pcs;
	int num_pcs;
	int cur_pc;
}model_t;

int pc_move(character_t *c, int y, int x);

model_t *init_model();
void free_model(model_t*);

#endif