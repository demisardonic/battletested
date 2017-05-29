#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "model.h"
#include "ui.h"
#include "util.h"

int read_map_from_file(const char* path, uint8_t *map);
int save_map_to_file(uint8_t *map, const char* path);

int main(int argc, char** argv){

	model_t *model = init_model();

	ui_t *ui = init_ui(model);
	
	//character arrays to store load and save paths.
	//"strings" initially set to "\0".
	char loadPath[BUFFER_SIZE];
	memset(loadPath, 0, BUFFER_SIZE);

	//Read commandline arguments if they exist
	if(argc > 1){
		int i;
		//strcpy(savePath, argv[1]);
		//strcpy(loadPath, argv[1]);
		for(i = 1; i < argc; i++){
			if(argv[i][0] == '-'){
				if(!strcmp("--load", argv[i]) || (strlen(argv[i]) == 2 && argv[i][1] == 'l')){ //Load argument
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
	
	//Attempt to read the map file.
	if(read_map_from_file(loadPath, model->map)){
		//If map cannot be read from file initialize blank map.
		int i;
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			model->map[i] = 0;
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
	
	update_valid_moves(model->map, model->player);
	//Draw the loaded or blank screen
	ui->draw();
	refresh();

	//invalid key means a new input can be parsed.
	int invalid;
	//Exit means input should no longer be read and program will close after key is handled
	int exit = 0;
	model->selY = model->player->y;
	model->selX = model->player->x;
	while(1){
		refresh();
		invalid = 0;
		//Pull keyboard input
		int key = getch();
		const char* keyLong = keyname(key);
		
		if(key == '^'){
			key = 0;
		}
		if(keyLong[0] == '^'){
			key = '^';
		}
		mvprintw(0, 0, "%d", model->player->moves);
		switch(key){
			case 'w':
				if(model->player->movement_map[yx_to_index(model->selY-1, model->selX)]){
					model->selY--;
				}
				break;
			case 's':
				if(model->player->movement_map[yx_to_index(model->selY+1, model->selX)]){
					model->selY++;
				}
				break;
			case 'a':
				if(model->player->movement_map[yx_to_index(model->selY, model->selX-1)]){
					model->selX--;
				}
				break;
			case 'd':
				if(model->player->movement_map[yx_to_index(model->selY, model->selX+1)]){
					model->selX++;
				}
				break;
			case ' ':
				pc_move(model->selY, model->selX);
				model->selY = model->player->y;
				model->selX = model->player->x;
				break;
			case 'q':
				//Quit program without saving
				exit = 1;
				break;
			case '^': //Handles ctrl key input
				switch(keyLong[1]){
					case 'Q':
						exit = 1;
						break;
					default:
						invalid = 1;
						break;
				}
				break;
			default:
				//The key input was not a invalid key and thus a new key should be input.
				invalid = 1;
				break;
		}
		
		//Read new input
		if(invalid){
			continue;
		}
		//break from input gathering while loop
		if(exit){
			break;
		}
		//Draw updated map and info tab
		ui->draw();
		refresh();
	}

	//delete ncurses window
	endwin();

	free_ui(ui);
	free_model(model);
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
		fprintf(stderr, "Failed to read header.\n");
		return 1;
	}
	buffer[6] = '\0';

	if(strcmp(buffer, "battle")){
		fprintf(stderr, "Header did not match.\n");
		return 1;
	}

	uint8_t version;
	if(!fread(&version, sizeof(char), 1, input)){
		fprintf(stderr, "Ininvalid file.\n");
		return 1;
	}
	
	if(version == 1){
		uint8_t tempMap[GAME_HEIGHT * GAME_WIDTH];
		int i;
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			if(!fread(&tempMap[i], sizeof(tempMap[i]), 1, input)){
				fprintf(stderr, "Failed to read tile %d\n", i);
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

