#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "selection.h"
#include "util.h"

void superimposeSelection(selection_t* sel, uint8_t* map, int x, int y){
	int row, col;
	for(row = 0; row < sel->height; row++){
		for(col = 0; col < sel->width; col++){
			map[(row + y) * GAME_WIDTH + (col + x)] = sel->val[row * sel->width + col];
		}
	}
}

void updateSelection(selection_t *sel, uint8_t* map, int x, int y, int width, int height){
	sel->x = x;
	sel->y = y;
	sel->width = width;
	sel->height = height;
	if(sel->val){
		free(sel->val);
	}
	sel->val = malloc(width*height);
	int row, col;
	for(row = 0; row < height; row++){
		for(col = 0; col < width; col++){
			sel->val[row * width + col] = map[(row + y) * GAME_WIDTH + (col + x)];
		}
	}
}

void freeSelection(selection_t* sel){
	free(sel->val);
}