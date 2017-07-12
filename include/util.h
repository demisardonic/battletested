#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define GAME_WIDTH 78
#define GAME_HEIGHT 19
#define TOP_BAR 0
#define BOTTOM_BAR_1 22
#define BOTTOM_BAR_2 23
#define FLOOR '.'
#define HALF_COVER 'x'
#define FULL_COVER ACS_CKBOARD
#define FILE_HEADER "btlmap"
#define BUFFER_SIZE (1<<8)
#define VERSION 1
#define LINE_CLEAR "                                                                                "

#define COLOR_DEFAULT 1
#define COLOR_PC 2
#define COLOR_MOVE 3
#define COLOR_DMOVE 4

#define LOG_FILE_PATH "btlog"

//UI MODES
#define MODE_GAME 1
#define MODE_SELECT_SQUAD 2

#define MAX_SQUAD_SIZE 5

void init_color_pairs();
int is_alphanumeric_char(char);
int is_path_char(char);
int yx_to_index(int y, int x);

size_t size_to_width(uint32_t size);
size_t size_to_height(uint32_t size);
size_t size_to_floors(uint32_t size);

void index_to_yx(int index, int *y, int *x);

bool in_range(uint32_t val, uint32_t min, uint32_t max);

#endif
