#ifndef UI_H
#define UI_H

#include <stdint.h>

#include "model.h"

typedef struct ui {
	const model_t *model;
	
	void (*draw)();
	void (*prompt)(char*, const char*, ...);
	void (*message)(const char*, ...);
} ui_t;

ui_t *init_ui(const model_t*);
void free_ui(ui_t*);

#endif