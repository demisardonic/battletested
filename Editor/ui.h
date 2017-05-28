#ifndef UI_H
#define UI_H

#include <stdint.h>
typedef struct ui ui_t;
struct ui{
	uint8_t *map;
	void (*draw)();
};

ui_t *init_ui();

void ui_message(const char*, ...);
void ui_prompt(char*, const char*, ...);

void free_ui(ui_t*);

#endif