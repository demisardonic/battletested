#ifndef UI_H
#define UI_H

#include <stdint.h>

#include "model.h"

typedef struct ui {
	const Model *model;
	uint8_t mode;
	
	void (*draw)();
	void (*prompt)(char*, const char*, ...);
	void (*message)(const char*, ...);
} ui_t;

ui_t *init_ui(const Model*);
void free_ui(ui_t*);

#endif