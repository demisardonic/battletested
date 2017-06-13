#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "character.h"
#include "model.h"
#include "util.h"

model_t *model;

int rotate_cur_pc(){
	model->cur_pc = (model->cur_pc + 1) % model->num_pcs;
	model->moveY = model->squad[model->cur_pc]->y;
	model->moveX = model->squad[model->cur_pc]->x;
	return model->cur_pc;
}

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
	model->cur_pc = -1;
	model->moveY = -1;
	model->moveX = -1;
	model->selection = 0;
	model->num_pcs = 0;
	
	for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
		model->char_loc[i] = NULL;
	}
	
	return model;
}

void free_model(model_t* model){
	int i;
	for(i = 0; i < model->num_pc_info; i++){
		delete model->pc_info[i];
	}
	delete[] model->pc_info;
	
	if(model->squad){
		for(i = 0; i < model->num_pcs; i++){
			free_character(model->squad[i]);
		}
	}
	
	free(model->squad);
	
	free(model->map);
	free(model);
}