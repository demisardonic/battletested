#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include <stdint.h>

#include "character.h"
#include "util.h"

class Character;
class Character_Info;

class Model{
	public:
		uint8_t *map;
		
		Character_Info **pc_info;
		int num_pc_info;
		
		std::vector<Character*> *squad;
		uint8_t num_pcs;
		
		int moveY;
		int moveX;
		int selection;
		int cur_pc;
		
		Character *char_loc[GAME_HEIGHT * GAME_WIDTH];
		
		Model();
		~Model();
};

int rotate_cur_pc();
int pc_move(Character *c, int y, int x);

#endif