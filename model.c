#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "character.h"
#include "model.h"
#include "util.h"

model_t *model;

int pc_move_left(){
	if(!model){
		return 1;
	}
	return move_character(model->map, model->player, model->player->y, model->player->x-1);
}

int pc_move_right(){
	if(!model){
		return 1;
	}
	return move_character(model->map, model->player, model->player->y, model->player->x+1);
}

int pc_move_up(){
	if(!model){
		return 1;
	}
	return move_character(model->map, model->player, model->player->y-1, model->player->x);
}

int pc_move_down(){
	if(!model){
		return 1;
	}
	return move_character(model->map, model->player, model->player->y+1, model->player->x);
}

model_t *init_model(){
	if(model){
		fprintf(stderr, "Model already initialized.\n");
		return model;
	}
	model = malloc(sizeof(model_t));
	model->map = malloc(GAME_HEIGHT * GAME_WIDTH);
	model->player = malloc(sizeof(character_t));
	model->player->x = 1;
	model->player->y = 1;
	model->player->c = '@';
	model->player->color = COLOR_PC;
	return model;
}

void free_model(model_t* model){
	if(!model){
		fprintf(stderr, "Model has not been initialized.\n");
	}
	free(model->map);
	free(model->player);
	free(model);
}