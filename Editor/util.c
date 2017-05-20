#include <ncurses.h>

#include "util.h"

//Creates the color pairs which are used
void init_color_pairs(){
	init_pair(COLOR_DEFAULT, COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOR_SELECTED, COLOR_WHITE, COLOR_RED);
}