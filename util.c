#include <ncurses.h>

#include "util.h"


void init_color_pairs(){
	init_pair(COLOR_DEFAULT, COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOR_PC, COLOR_RED, COLOR_YELLOW);
	init_pair(COLOR_SELECTED, COLOR_BLACK, COLOR_BLUE);
}