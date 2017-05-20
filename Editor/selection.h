#ifndef SELECTION_H
#define SELECTION_H

#include <stdint.h>

typedef struct selection{
   int x;
    int y;
	int height;
	int width;
	uint8_t *val;
} selection_t;

void superimposeSelection(selection_t* sel, uint8_t* map, int x, int y);

void updateSelection(selection_t *sel, uint8_t* map, int x, int y, int width, int height);

void freeSelection(selection_t* sel);

#endif