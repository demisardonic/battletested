#include <stdlib.h>
#include <stdint.h>

#include "model.h"
#include "util.h"

model_t *model;

model_t *init_model(){
	model = malloc(sizeof(model_t));
	model->map = malloc(GAME_HEIGHT * GAME_WIDTH);
	
	model->selectedX = 0;
	model->selectedY = 0;
	model->selectedWidth = 1;
	model->selectedHeight = 1;
	return model;
}

void free_model(model_t* model){
	free(model->map);
	free(model);
}