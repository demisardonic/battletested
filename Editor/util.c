#include <ncurses.h>

#include "util.h"

//Creates the color pairs which are used
void init_color_pairs(){
	init_pair(COLOR_DEFAULT, COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOR_SELECTED, COLOR_WHITE, COLOR_RED);
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