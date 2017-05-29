#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "character.h"
#include "model.h"
#include "util.h"

model_t *model;

int pc_move(int y, int x){
	if(!model){
		return 1;
	}
	return move_character(model->map, model->player, y, x);
}

model_t *init_model(){
	if(model){
		fprintf(stderr, "Model already initialized.\n");
		return model;
	}
	model = (model_t *)malloc(sizeof(model_t));
	model->map = (uint8_t *)malloc(GAME_HEIGHT * GAME_WIDTH);
	model->player = init_character();
	model->player->c = '@';
	model->player->color = COLOR_PC;
	return model;
}

void free_model(model_t* model){
	if(!model){
		fprintf(stderr, "Model has not been initialized.\n");
	}
	free(model->map);
	free_character(model->player);
	free(model);
}