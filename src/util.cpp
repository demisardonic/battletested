#include <string>

#include <ncurses.h>

#include "util.h"

//Creates the color pairs which are used
void init_color_pairs(){
	if(can_change_color()){
		//init_color(COLOR_RED, 1000, 0, 0);
	}
	init_pair(COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_PC, COLOR_WHITE, COLOR_RED);
	init_pair(COLOR_MOVE, COLOR_WHITE, COLOR_GREEN);
	init_pair(COLOR_DMOVE, COLOR_WHITE, COLOR_CYAN);
}

//Returns 1 if the character is alphanumeric
int is_alphanumeric_char(char val){
	if(val >= 'a' && val <= 'z'){
		return 1;
	}
	if(val >= 'A' && val <= 'Z'){
		return 1;
	}
	if(val >= '0' && val <= '9'){
		return 1;
	}
	return 0;
}

//Returns 1 if the char is either '.', '_', '-', '/', or alphanumeric
int is_path_char(char val){
	if(val == '.' || val == '_' || val == '-' || val == '/'){
		return 1;
	}
	return is_alphanumeric_char(val);
}

int yx_to_index(int y, int x){
	return y * GAME_WIDTH + x;
}

void index_to_yx(int index, int *y, int *x){
	*y = index/GAME_WIDTH;
	*x = index%GAME_WIDTH;
}

std::string trim(const std::string str){
	size_t first = str.find_first_not_of(' ');
  if (std::string::npos == first){
      return str;
  }
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}
