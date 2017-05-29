#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#include "character.h"

typedef struct model model_t;
struct model{
	uint8_t *map;
	character_t *player;
	int selY;
	int selX;
};

int pc_move(int y, int x);

model_t *init_model();
void free_model(model_t*);

#endif