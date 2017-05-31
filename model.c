#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "character.h"
#include "model.h"
#include "util.h"

model_t *model;

int pc_move(character_t *c, int y, int x){
	if(!model){
		return 0;
	}
	return move_character(model, c, y, x);
}


model_t *init_model(){
	if(model){
		fprintf(stderr, "Model already initialized.\n");
		return model;
	}
	int i;
	model = (model_t *)malloc(sizeof(model_t));
	model->num_pcs = 5;
	model->cur_pc = 0;
	for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
		model->char_loc[i] = NULL;
	}
	model->pcs = (character_t **) malloc(sizeof(character_t *) * model->num_pcs);
	for(i = 0; i < model->num_pcs; i++){
		model->pcs[i] = init_character();
		model->char_loc[yx_to_index(model->pcs[i]->y, model->pcs[i]->x)] = model->pcs[i];
	}
	return model;
}

void free_model(model_t* model){
	int i;
	for(i = 0; i < model->num_pcs; i++){
		free_character(model->pcs[i]);
	}
	free(model->pcs);
	
	free(model->map);
	free(model);
}