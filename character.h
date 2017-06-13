#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

#include <stdint.h>

#include "model.h"

class Model;

class Character_Info{
	public:
		std::string f_name;
		std::string l_name;
		uint8_t stats[7];
		uint8_t in_squad;
		
		Character_Info();
		~Character_Info();
};

class Character{
	public:
		int x;
		int y;
		int speed;
		char c;
		int color;
		uint8_t *movement_map;
		int turns;
		Character_Info *info;
		
		Character(Character_Info*);
		~Character();
};

void update_valid_moves(Character**, uint8_t*, Character*);
int move_character(Model *, Character*, int, int);

#endif