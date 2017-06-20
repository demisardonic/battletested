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
	this->num_pcs = 0;
	int i;
	for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
		this->char_loc[i] = NULL;
	}
	model = this;
}

Model::~Model(){
	while((*this->squad).size()>0){
		Character *temp = (*this->squad).back();
		(*this->squad).pop_back();
		delete temp;
	}
	delete this->squad;
	int i;
	for(i = this->num_pc_info - 1; i >= 0; i--){
		delete this->pc_info[i];
	}
	delete [] this->pc_info;
	free(this->map);
}
