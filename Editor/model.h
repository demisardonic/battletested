#ifndef MODEL_H
#define MODEL_H


typedef struct model model_t;
struct model{
	uint8_t *map;
	
	int selectedX;
	int selectedY;
	int selectedWidth;
	int selectedHeight;
};

model_t *init_model();
void free_model(model_t*);

#endif