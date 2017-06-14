#include <vector>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "character.h"
#include "model.h"
#include "util.h"

Model *model;

int rotate_cur_pc(){
	model->cur_pc = (model->cur_pc + 1) % (*model->squad).size();
	model->moveY = (*model->squad)[model->cur_pc]->y;
	model->moveX = (*model->squad)[model->cur_pc]->x;
	return model->cur_pc;
}

int pc_move(Character *c, int y, int x){
	if(!model){
		return 0;
	}
	return move_character(model, c, y, x);
}

Model::Model(){
	this->squad = new std::vector<Character *>;
	this->cur_pc = -1;
	this->moveY = -1;
	this->moveX = -1;
	this->selection = 0;
	this->num_pcs = 0;
	int i;
	for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
		this->char_loc[i] = NULL;
	}
	model = this;
}

Model::~Model(){
	while((*model->squad).size()>0){
		Character *temp = (*model->squad).back();
		(*model->squad).pop_back();
		delete temp;
	}
	delete model->squad;
	int i;
	for(i = model->num_pc_info - 1; i >= 0; i--){
		delete model->pc_info[i];
	}
	delete [] model->pc_info;
	free(this->map);
	delete this->squad;
}