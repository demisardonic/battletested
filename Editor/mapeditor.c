#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "selection.h"
#include "util.h"

int set_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile);
int set_selection_rotate(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight);
int draw_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight);
int draw_info(int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile);
int draw_boarder();
int draw_map(uint8_t map[], int x, int y);
void ui_prompt(const char *pre, char *input);
void ui_message(const char *message, ...);
uint8_t* read_map_from_file(const char* path);
int save_map_to_file(uint8_t *map, const char* path);

int main(int argc, char** argv){
	//initial start values for selected tiles
	int selectedX = 0;
	int selectedY = 0;
	int selectedWidth = 1;
	int selectedHeight = 1;
	int selectedTile = 0;

	//Holds the tile values of the map
	uint8_t *map = NULL;
	uint8_t *tmpMap = NULL;

	//character arrays to store load and save paths.
	//"strings" initially set to "\0".
	char loadPath[BUFFER_SIZE];
	char savePath[BUFFER_SIZE];
	char tempPath[BUFFER_SIZE];
	
	int i;
	for(i = 0; i < BUFFER_SIZE; i++){
		loadPath[i] = '\0';
		savePath[i] = '\0';
		tempPath[i] = '\0';
	}

	//Clip board contains a rectangle of tiles 
	//which can by copied and pasted into different locations
	selection_t clipboard;
	clipboard.val = NULL;

	if(argc > 1){
		int i;
		//strcpy(savePath, argv[1]);
		//strcpy(loadPath, argv[1]);
		for(i = 1; i < argc; i++){
			if(argv[i][0] == '-'){
				if(!strcmp("--save", argv[i]) || (strlen(argv[i]) == 2 && argv[i][1] == 's')){ //Save argument
					if(i+1 < argc){
						strcpy(savePath, argv[i+1]);
						i++;
					}else{
						fprintf(stderr, "Incorrect usage of --save.");
						return 1;
					}
				}else if(!strcmp("--load", argv[i]) || (strlen(argv[i]) == 2 && argv[i][1] == 'l')){ //Load argument
					if(i+1 < argc){
						strcpy(loadPath, argv[i+1]);
						i++;
					}else{
						fprintf(stderr, "Incorrect usage of --load.");
						return 1;
					}
				}else if(!strcmp("--version", argv[i]) || (strlen(argv[i]) == 2 && argv[i][1] == 'v')){ //Version argument
					if(i+1 < argc){
						strcpy(loadPath, argv[i+1]);
						i++;
					}else{
						fprintf(stderr, "Incorrect usage of --version.");
						return 1;
					}
				}
			}
		}
	}
	//If the loadPath was not changed via commandline arguments.
	if(loadPath[0] != '\0'){
		map = read_map_from_file(loadPath);
	}
	//If map cannot be read from file or no path is provided initialize blank map.
	if(!map){
		uint8_t blank[GAME_HEIGHT * GAME_WIDTH];
		int i;
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			blank[i] = 0;
		}
		map = blank;
	}
	//NCurses screen initialization
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	init_color_pairs();

	//Draw the loaded or blank screen
	draw_boarder();
	draw_map(map, selectedX, selectedY);
	draw_info(selectedX, selectedY, selectedWidth, selectedHeight, selectedTile);
	draw_selection(map, selectedX, selectedY, selectedWidth, selectedHeight);
	refresh();

	//Valid key means a new input can be parsed.
	int valid;
	//Exit means input should no longer be read and program will close after key is handled
	int exit = 0;
	while(1){
		valid = 1;
		//Pull keyboard input
		int key = getch();
		const char* keyLong = keyname(key);
		
		if(key == '^'){
			key = 0;
		}
		
		if(keyLong[0] == '^'){
			key = '^';
		}
		switch(key){
			case 'w':
				//Move selection up one index.
				selectedY = selectedY > 0 ? selectedY - 1 : GAME_HEIGHT - 1;
				break;
			case 's':
				//Move selection down one index.
				selectedY = selectedY < GAME_HEIGHT - 1 ? selectedY + 1 : 0;
				break;
			case 'a':
				//Move selection left one index.
				selectedX = selectedX > 0 ? selectedX - 1 : GAME_WIDTH - 1;
				break;
			case 'd':
				//Move selection right one index.
				selectedX = selectedX <  GAME_WIDTH - 1 ? selectedX + 1 : 0;
				break;
			case 'W':
				//Make selection one tile larger vertically.
				selectedHeight = selectedHeight > 1 ? selectedHeight - 1 : 1;
				break;
			case 'S':
				//Make selection one tile smaller vertically.
				selectedHeight = selectedHeight < GAME_HEIGHT - 1 ? selectedHeight + 1 : GAME_HEIGHT;
				break;
			case 'A':
				//Make selection one tile smaller horizontally.
				selectedWidth = selectedWidth > 1 ? selectedWidth - 1 : 1;
				break;
			case 'D':
				//Make selection one tile larger horizontally.
				selectedWidth = selectedWidth <  GAME_WIDTH - 1 ? selectedWidth + 1 : GAME_WIDTH;
				break;
			case ' ': //Space
				//Rotate every tile within the selection
				set_selection_rotate(map, selectedX, selectedY, selectedWidth, selectedHeight);
				break;
			case '	': //Tab
				//Rotate selected tile
				selectedTile = (selectedTile+1) % 3;
				break;
			case 'e':
				//Set every tile within selection to selected tile
				set_selection(map, selectedX, selectedY, selectedWidth, selectedHeight, selectedTile);
				break;
			case 'q':
				//Quit program without saving
				exit = 1;
				break;
			case '^': //Handles ctrl key input
				switch(keyLong[1]){
					case 'S': //save
					{
						int textKey;
						int index = 0;
						int escape = 0;
						curs_set(1);
						ui_message("Save: ");
						textKey = getch();
						while(textKey != '\n'){
							if(is_path_char(textKey)){
								addch(textKey);
								tempPath[index] = textKey;
								index++;
								tempPath[index] = '\0';
							}else if(textKey == KEY_BACKSPACE){
								index--;
								tempPath[index] = '\0';
								ui_message("Save: %s", tempPath);
							}else if(textKey == 27){
								escape = 1;
								curs_set(0);
								tempPath[0] = '\0';
								break;
							}
							textKey = getch();
						}
						if(!escape){
							if(index == 0){
								strcpy(tempPath, savePath);
							}else{
								tempPath[index] = '\0';
							}
							
							if(!save_map_to_file(map, tempPath)){
								ui_message("Saved to %s.", tempPath);
							}else{
								ui_message("Failed to save to %s", tempPath);
							}
						}else{
							ui_message(NULL);
						}
						curs_set(0);
						break;
					}
					case 'O': //open
					{
						int textKey;
						int index = 0;
						int escape = 0;
						curs_set(1);
						mvprintw(0, 0, LINE_CLEAR);
						mvprintw(0, 0, "Open: ");
						textKey = getch();
						while(textKey != '\n'){
							if(is_path_char(textKey)){
								addch(textKey);
								tempPath[index] = textKey;
								index++;
								tempPath[index] = '\0';
							}else if(textKey == KEY_BACKSPACE){
								index--;
								tempPath[index] = '\0';
								ui_message("Open: %s", tempPath);
							}else if(textKey == 27){
								escape = 1;
								curs_set(0);
								tempPath[0] = '\0';
								break;
							}
							textKey = getch();
						}
						if(!escape){
							if(index == 0){
								strcpy(tempPath, loadPath);
							}else{
								tempPath[index] = '\0';
							}
							tmpMap = read_map_from_file(tempPath);
							if(tmpMap){
								ui_message("Opened %s", tempPath);
								map = tmpMap;
								strcpy(loadPath, tempPath);
							}else{
								ui_message("Failed to opened %s", tempPath);
							}
						}else{
							ui_message(NULL);
						}
						curs_set(0);
						break;
					}
					case 'Z': //undo
						valid = 0;
						break;
					case 'Y': //redo
						valid = 0;
						break;
					case 'X': //cut
						updateSelection(&clipboard, map, selectedX, selectedY, selectedWidth, selectedHeight);
						set_selection(map, selectedX, selectedY, selectedWidth, selectedHeight, 0);
						break;
					case 'C': //copy
						updateSelection(&clipboard, map, selectedX, selectedY, selectedWidth, selectedHeight);
						break;
					case 'V': //paste
						superimposeSelection(&clipboard, map, selectedX, selectedY);
						break;
					case 'Q':
						exit = 1;
						break;
					default:
						valid = 0;
						break;
				}
				break;
			case 0: default:
				//The key input was not a valid key and thus a new key should be input.
				valid = 0;
				break;
		}
		//break from input gathering while loop
		if(exit){
			break;
		}
		//Read new input
		if(!valid){
			continue;
		}

		//Draw updated map and info tab
		draw_map(map, selectedX, selectedY);
		draw_info(selectedX, selectedY, selectedWidth, selectedHeight, selectedTile);
		draw_selection(map, selectedX, selectedY, selectedWidth, selectedHeight);
		refresh();
	}

	//delete ncurses window
	endwin();

	if(exit == 2 && savePath[0] != '\0'){
		save_map_to_file(map, savePath);
	}

	freeSelection(&clipboard);
	return 0;
}

//Set every tile within selection to selectedTile
int set_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile){
	int i, j;
	for(i = 0; i < selectedHeight; i++){
		for(j = 0; j < selectedWidth; j++){
			map[((i + selectedY) % GAME_HEIGHT)*GAME_WIDTH+((j + selectedX) % GAME_WIDTH)] = selectedTile;
		}
	}
	return 0;
}

//Rotate every available tile
int set_selection_rotate(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight){
	int i, j;
	for(i = 0; i < selectedHeight; i++){
		for(j = 0; j < selectedWidth; j++){
			map[((i + selectedY) % GAME_HEIGHT)*GAME_WIDTH+((j + selectedX) % GAME_WIDTH)] = (map[((i + selectedY) % GAME_HEIGHT)*GAME_WIDTH+((j + selectedX) % GAME_WIDTH)]+1) % 3;
		}
	}
	return 0;
}

//Draw colored selection box
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

//Print two lines of editor information (current x, y, width, height, and tile)
int draw_info(int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile){
	mvprintw(BOTTOM_BAR_1, 0, LINE_CLEAR);
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
	mvprintw(BOTTOM_BAR_2, 0, LINE_CLEAR);
	mvprintw(BOTTOM_BAR_2, 0, "Current Width: %d, Current Height: %d", selectedWidth, selectedHeight);
	return 0;
}

//Draw decorate boarder around map
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

//Draw entire map
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

void ui_prompt(const char *pre, char *input){
	int textKey;
	int index = 0;
	int escape = 0;
	curs_set(1);
	mvprintw(0, 0, LINE_CLEAR);
	mvprintw(0, 0, "%s", pre);
	textKey = getch();
	while(textKey != '\n'){
		if(is_path_char(textKey)){
			addch(textKey);
			input[index] = textKey;
			index++;
			input[index] = '\0';
		}else if(textKey == KEY_BACKSPACE){
			index--;
			input[index] = '\0';
			mvprintw(0, 0, LINE_CLEAR);
			mvprintw(0, 0, "Open: %s", input);
		}else if(textKey == 27){
			escape = 1;
			curs_set(0);
			input[0] = '\0';
			break;
		}
		textKey = getch();
	}
	if(!escape){
		if(index == 0){
			input[0] = '\0';
		}else{
			input[index] = '\0';
		}
	}
	curs_set(0);
}

void ui_message(const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	mvprintw(0, 0, LINE_CLEAR);
	mvprintw(0, 0, fmt, args);
	va_end(args);
}

//Output map array generated from reading the given file path
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

//Save the map array to file
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

