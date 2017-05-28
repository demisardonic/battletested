#ifndef UI_H
#define UI_H

#include <stdint.h>
typedef struct ui ui_t;
struct ui{
	uint8_t *map;
	
	int selectedX;
	int selectedY;
	int selectedWidth;
	int selectedHeight;
	int selectedTile;
	
	void (*draw)();
	void (*prompt)(char*, const char*, ...);
	void (*message)(const char*, ...);
};

ui_t *init_ui();
void free_ui(ui_t*);

#endif