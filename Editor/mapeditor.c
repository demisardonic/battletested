#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "ui.h"
#include "selection.h"
#include "util.h"

int set_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight, uint8_t selectedTile);
int set_selection_rotate(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight);
int read_map_from_file(const char* path, uint8_t *map);
int save_map_to_file(uint8_t *map, const char* path);

int main(int argc, char** argv){
	//initial start values for selected tiles
	
	
	ui_t *ui = init_ui();
	ui->draw();
	
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
		if(!read_map_from_file(loadPath, ui->map)){
			//If map cannot be read from file or no path is provided initialize blank map.
			int i;
			for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
				ui->map[i] = 0;
			}
		}
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
	ui->draw();
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
				ui->selectedY = ui->selectedY > 0 ? ui->selectedY - 1 : GAME_HEIGHT - 1;
				break;
			case 's':
				//Move selection down one index.
				ui->selectedY = ui->selectedY < GAME_HEIGHT - 1 ? ui->selectedY + 1 : 0;
				break;
			case 'a':
				//Move selection left one index.
				ui->selectedX = ui->selectedX > 0 ? ui->selectedX - 1 : GAME_WIDTH - 1;
				break;
			case 'd':
				//Move selection right one index.
				ui->selectedX = ui->selectedX <  GAME_WIDTH - 1 ? ui->selectedX + 1 : 0;
				break;
			case 'W':
				//Make selection one tile larger vertically.
				ui->selectedHeight = ui->selectedHeight > 1 ? ui->selectedHeight - 1 : 1;
				break;
			case 'S':
				//Make selection one tile smaller vertically.
				ui->selectedHeight = ui->selectedHeight < GAME_HEIGHT - 1 ? ui->selectedHeight + 1 : GAME_HEIGHT;
				break;
			case 'A':
				//Make selection one tile smaller horizontally.
				ui->selectedWidth = ui->selectedWidth > 1 ? ui->selectedWidth - 1 : 1;
				break;
			case 'D':
				//Make selection one tile larger horizontally.
				ui->selectedWidth = ui->selectedWidth <  GAME_WIDTH - 1 ? ui->selectedWidth + 1 : GAME_WIDTH;
				break;
			case ' ': //Space
				//Rotate every tile within the selection
				set_selection_rotate(ui->map, ui->selectedX, ui->selectedY, ui->selectedWidth, ui->selectedHeight);
				break;
			case '	': //Tab
				//Rotate selected tile
				ui->selectedTile = (ui->selectedTile+1) % 3;
				break;
			case 'e':
				//Set every tile within selection to selected tile
				set_selection(ui->map, ui->selectedX, ui->selectedY, ui->selectedWidth, ui->selectedHeight, ui->selectedTile);
				break;
			case 'q':
				//Quit program without saving
				exit = 1;
				break;
			case '^': //Handles ctrl key input
				switch(keyLong[1]){
					case 'S': //save
						ui->prompt(tempPath, "Save: ");
						if(!save_map_to_file(ui->map, tempPath)){
							ui->message("Save success.");
						}else{
							ui->message("Save failure.");
						}
						break;
					case 'O': //open
						ui->prompt(tempPath, "Open: ");
						if(!read_map_from_file(tempPath, ui->map)){
							ui->message("Open success.");
							strcpy(loadPath, tempPath);
						}else{
							ui->message("Open failure.");
						}
						break;
					case 'Z': //undo
						valid = 0;
						break;
					case 'Y': //redo
						valid = 0;
						break;
					case 'X': //cut
						updateSelection(&clipboard, ui->map, ui->selectedX, ui->selectedY, ui->selectedWidth, ui->selectedHeight);
						set_selection(ui->map, ui->selectedX, ui->selectedY, ui->selectedWidth, ui->selectedHeight, 0);
						break;
					case 'C': //copy
						updateSelection(&clipboard, ui->map, ui->selectedX, ui->selectedY, ui->selectedWidth, ui->selectedHeight);
						break;
					case 'V': //paste
						superimposeSelection(&clipboard, ui->map, ui->selectedX, ui->selectedY);
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
		ui->draw();
		refresh();
	}

	//delete ncurses window
	endwin();

	if(exit == 2 && savePath[0] != '\0'){
		save_map_to_file(ui->map, savePath);
	}

	freeSelection(&clipboard);
	free_ui(ui);
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

//Output map array generated from reading the given file path
int read_map_from_file(const char* path, uint8_t *map){
	
	FILE *input = fopen(path, "r");
	if(!input){
		fprintf(stderr, "File %s does not exist.\n", path);
		return 1;
	}

 	char buffer[BUFFER_SIZE];
	if(fread(buffer, sizeof(char), 6, input) != 6){
		return 1;
	}
	buffer[6] = '\0';

	if(strcmp(buffer, "battle")){
		fprintf(stderr, "Header did not match.");
		return 1;
	}

	uint8_t version;
	if(!fread(&version, sizeof(char), 1, input)){
		fprintf(stderr, "Invalid file.\n");
		return 1;
	}
	
	if(version == 1){
		uint8_t tempMap[GAME_HEIGHT * GAME_WIDTH];
		int i;
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			if(!fread(&tempMap[i], sizeof(tempMap[i]), 1, input)){
				return 1;
			}
		}
		//The map is written to after reading the entire map so that
		//the ui map is not corrupted if only a partial map is read.
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			map[i] = tempMap[i];
		}
	}
	return 0;
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

