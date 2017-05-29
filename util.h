#ifndef UTIL_H
#define UTIL_H

#define GAME_WIDTH 78
#define GAME_HEIGHT 19
#define TOP_BAR 0
#define BOTTOM_BAR_1 22
#define BOTTOM_BAR_2 23
#define FLOOR '.'
#define HALF_COVER 'x'
#define FULL_COVER ACS_CKBOARD
#define FILE_HEADER "battle"
#define BUFFER_SIZE (1<<8)
#define VERSION 1
#define LINE_CLEAR "                                                                                "

#define COLOR_DEFAULT 1
#define COLOR_PC 2


void init_color_pairs();
int is_alphanumeric_char(char);
int is_path_char(char);
int yx_to_index(int y, int x);

#endif