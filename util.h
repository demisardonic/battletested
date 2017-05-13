#ifndef UTIL_H
#define UTIL_H

#include <ncurses.h>

#define GAME_WIDTH 78
#define GAME_HEIGHT 19
#define TOP_BAR 0
#define BOTTOM_BAR_1 22
#define BOTTOM_BAR_2 23
#define FLOOR '.'
#define HALF_COVER ACS_DIAMOND
#define FULL_COVER ACS_CKBOARD
#define FILE_HEADER "battle"
#define BUFFER_SIZE (1<<8)
#define VERSION 1

#define COLOR_DEFAULT 0
#define COLOR_PC 1
#define COLOR_SELECTED 2

void init_color_pairs();
#endif