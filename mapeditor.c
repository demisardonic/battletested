#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "util.h"
#include "logger.h"

int set_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile);
int set_selection_rotate(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight);
int draw_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight);
int draw_info(int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile);
int draw_boarder();
int draw_map(uint8_t map[], int x, int y);
uint8_t* read_map_from_file(const char* path);
int save_map_to_file(uint8_t *map, const char* path);
void init_color_pairs();

int main(int argc, char** argv){
	int selectedX = 0;
	int selectedY = 0;
	int selectedWidth = 1;
	int selectedHeight = 1;
	int selectedTile = 0;
	char loadPath[BUFFER_SIZE];
	char savePath[BUFFER_SIZE];
	
	uint8_t *map = NULL;
	if(argc > 1){
		int i;
		strcpy(savePath, argv[1]);
		strcpy(loadPath, argv[1]);
		for(i = 1; i < argc; i++){
			if(argv[i][0] == '-'){
				if(!strcmp("--save", argv[i]) || (strlen(argv[i]) == 2 && argv[i][1] == 's')){
					if(i+1 < argc){
						strcpy(savePath, argv[i+1]);
						i++;
					}else{
						fprintf(stderr, "Incorrect usage of --load.");
						return 1;
					}
				}else if(!strcmp("--load", argv[i]) || (strlen(argv[i]) == 2 && argv[i][1] == 'l')){
					if(i+1 < argc){
						strcpy(loadPath, argv[i+1]);
						i++;
					}else{
						fprintf(stderr, "Incorrect usage of --load.");
						return 1;
					}
				}
			}
		}
	}
	map = read_map_from_file(loadPath);
	if(!map){
		uint8_t blank[GAME_HEIGHT * GAME_WIDTH];
		int i;
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			blank[i] = 0;
		}
		map = blank;
	}

	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	init_color_pairs();

	draw_boarder();
	draw_map(map, selectedX, selectedY);
	draw_info(selectedX, selectedY, selectedWidth, selectedHeight, selectedTile);
	draw_selection(map, selectedX, selectedY, selectedWidth, selectedHeight);
	refresh();
	
	int valid;
	int exit = 0;
	while(1){
		valid = 1;
		int key = getch();
		switch(key){
			case 'w':
				selectedY = selectedY > 0 ? selectedY - 1 : GAME_HEIGHT - 1;
				break;
			case 's':
				selectedY = selectedY < GAME_HEIGHT - 1 ? selectedY + 1 : 0;
				break;
			case 'a':
				selectedX = selectedX > 0 ? selectedX - 1 : GAME_WIDTH - 1;
				break;
			case 'd':
				selectedX = selectedX <  GAME_WIDTH - 1 ? selectedX + 1 : 0;
				break;
			case 'W':
				selectedHeight = selectedHeight > 1 ? selectedHeight - 1 : 1;
				break;
			case 'S':
				selectedHeight = selectedHeight < GAME_HEIGHT - 1 ? selectedHeight + 1 : GAME_HEIGHT;
				break;
			case 'A':
				selectedWidth = selectedWidth > 1 ? selectedWidth - 1 : 1;
				break;
			case 'D':
				selectedWidth = selectedWidth <  GAME_WIDTH - 1 ? selectedWidth + 1 : GAME_WIDTH;
				break;
			case ' ':
				set_selection_rotate(map, selectedX, selectedY, selectedWidth, selectedHeight);
				break;
			case '	':
				selectedTile = (selectedTile+1) % 3;
				break;
			case 'e':
				set_selection(map, selectedX, selectedY, selectedWidth, selectedHeight, selectedTile);
				break;
			case 'q':
				exit = 1;
				break;
			case 19: //Ctrl-s save
				exit = 2;
				break;
			default:  
				valid = 0;
				break;
		}
		if(exit){
			break;
		}
		if(!valid){
			continue;
		}
		draw_map(map, selectedX, selectedY);
		draw_info(selectedX, selectedY, selectedWidth, selectedHeight, selectedTile);
		draw_selection(map, selectedX, selectedY, selectedWidth, selectedHeight);
		refresh();
	}

	//delete ncurses window
	endwin();

	if(exit == 2){
		save_map_to_file(map, savePath);
	}
	return 0;
}

int set_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile){
	int i, j;
	for(i = 0; i < selectedHeight; i++){
		for(j = 0; j < selectedWidth; j++){
			map[((i + selectedY) % GAME_HEIGHT)*GAME_WIDTH+((j + selectedX) % GAME_WIDTH)] = selectedTile;
		}
	}
	return 0;
}

int set_selection_rotate(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight){
	int i, j;
	for(i = 0; i < selectedHeight; i++){
		for(j = 0; j < selectedWidth; j++){
			map[((i + selectedY) % GAME_HEIGHT)*GAME_WIDTH+((j + selectedX) % GAME_WIDTH)] = (map[((i + selectedY) % GAME_HEIGHT)*GAME_WIDTH+((j + selectedX) % GAME_WIDTH)]+1) % 3;
		}
	}
	return 0;
}

int draw_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight){
	int i, j;
	attron(COLOR_PAIR(COLOR_SELECTED));
	for(i = 0; i < selectedHeight; i++){
		for(j = 0; j < selectedWidth; j++){
			move(2+ ((i + selectedY) % GAME_HEIGHT), 1 + ((j + selectedX) % GAME_WIDTH));
			uint8_t val = map[((i + selectedY) % GAME_HEIGHT)*GAME_WIDTH+((j + selectedX) % GAME_WIDTH)];
			if(val == 0){
				addch(FLOOR);
			}else if (val == 1){
				addch(HALF_COVER);
			}else if(val == 2){
				addch(FULL_COVER);
			}else{
				addch(val);	
			}

		}
	}
	attroff(COLOR_PAIR(COLOR_SELECTED));
	return 0;
}

int draw_info(int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile){
	mvprintw(BOTTOM_BAR_1, 0, "Current X: %d, Current Y: %d, Current Tile: ", selectedX, selectedY);
	if(selectedTile == 0){
		addch(FLOOR);
	}else if (selectedTile == 1){
		addch(HALF_COVER);
	}else if(selectedTile == 2){
		addch(FULL_COVER);
	}else{
		addch(selectedTile);	
	}
	mvprintw(BOTTOM_BAR_2, 0, "Current Width: %d, Current Height: %d", selectedWidth, selectedHeight);
	return 0;
}

int draw_boarder(){
	attron(COLOR_PAIR(COLOR_DEFAULT));
	int i, j;
	for(i = 1; i < GAME_HEIGHT+1; i++){
		mvaddch(i+1, 0, ACS_VLINE);
		mvaddch(i+1, GAME_WIDTH+1, ACS_VLINE);
	}
	for(j = 1; j < GAME_WIDTH+1; j++){
		mvaddch(0+1, j, ACS_HLINE);
		mvaddch(GAME_HEIGHT+1+1, j, ACS_HLINE);
	}
	mvaddch(0+1,0, ACS_ULCORNER);
	mvaddch(0+1,GAME_WIDTH+1, ACS_URCORNER);
	mvaddch(GAME_HEIGHT+1+1,0, ACS_LLCORNER);
	mvaddch(GAME_HEIGHT+1+1,GAME_WIDTH+1, ACS_LRCORNER);
	attroff(COLOR_PAIR(COLOR_DEFAULT));
	return 0;
}

int draw_map(uint8_t map[], int x, int y){
	int i, j;
	for(i = 0; i < GAME_HEIGHT; i++){
		for (j = 0; j < GAME_WIDTH; j++){
			move(2+i, 1+j);
			uint8_t val = map[i*GAME_WIDTH+j];
			if(val == 0){
				addch(FLOOR);
			}else if (val == 1){
				addch(HALF_COVER);
			}else if(val == 2){
				addch(FULL_COVER);
			}else{
				addch(val);	
			}
		}
	}
	return 0;
}

uint8_t* read_map_from_file(const char* path){
	FILE *input = fopen(path, "r");
	if(!input){
		fprintf(stderr, "File %s does not exist.\n", path);
		return NULL;
	}

 	char buffer[BUFFER_SIZE];
	if(fread(buffer, sizeof(char), 6, input) != 6){
		return NULL;
	}
	buffer[6] = '\0';

	if(strcmp(buffer, "battle")){
		fprintf(stderr, "Header did not match.");
		return NULL;
	}

	uint8_t version;
	if(!fread(&version, sizeof(char), 1, input)){
		fprintf(stderr, "Invalid file.\n");
		return NULL;
	}

	static uint8_t map[GAME_WIDTH * GAME_HEIGHT];
	if(version == 1){
		int i;
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			fread(&map[i], sizeof(map[i]), 1, input);
		}
	}

	return map;
}

int save_map_to_file(uint8_t *map, const char* path){
	FILE* outfile = fopen(path, "w");
	if(!outfile){
		fprintf(stderr, "Could not save file: %s\n", path);
		return 1;
	}
	fwrite(FILE_HEADER, 6, 1, outfile);

	uint8_t ver = VERSION;
	fwrite(&ver, 1, 1, outfile);
	
	fwrite(map, sizeof(*map), GAME_WIDTH * GAME_HEIGHT, outfile);
	fclose(outfile);
	return 0;
}

void init_color_pairs(){
	init_pair(COLOR_DEFAULT, COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOR_PC, COLOR_RED, COLOR_YELLOW);
	init_pair(COLOR_SELECTED, COLOR_BLACK, COLOR_BLUE);
}