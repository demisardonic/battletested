#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "model.h"
#include "ui.h"
#include "util.h"

int read_map_from_file(const char* path, uint8_t *map);
int save_map_to_file(uint8_t *map, const char* path);

int main(int argc, char** argv){
	srand(time(NULL));

	//character arrays to store load and save paths.
	//"strings" initially set to "\0".
	char loadPath[BUFFER_SIZE];
	memset(loadPath, 0, BUFFER_SIZE);
	
	uint32_t seed = 0;

	//Read commandline arguments if they exist
	if(argc > 1){
		int i;
		//strcpy(savePath, argv[1]);
		//strcpy(loadPath, argv[1]);
		for(i = 1; i < argc; i++){
			if(argv[i][0] == '-'){
				if(!strcmp("--load", argv[i]) || (strlen(argv[i]) == 2 && argv[i][1] == 'l')){ //Load argument
					if(i+1 < argc){
						strcpy(loadPath, argv[++i]);
					}else{
						fprintf(stderr, "Incorrect usage of --load.");
						return 1;
					}
				}else if(!strcmp("--seed", argv[i])){ //Load argument
					if(i+1 < argc){
						seed = atoi(argv[++i]);
					}else{
						fprintf(stderr, "Incorrect usage of --load.");
						return 1;
					}
				}
			}
		}
	}
	
	if(seed){
		srand(seed);
	}else{
		srand(time(NULL));
	}
	
	uint8_t *map = (uint8_t *)malloc(GAME_HEIGHT * GAME_WIDTH);
	model_t *model = init_model();
	model->map = map;
	ui_t *ui = init_ui(model);
	
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
	//keypad(stdscr, TRUE);
	init_color_pairs();
	int i;
	for(i = 0; i < model->num_pcs; i++){
		update_valid_moves(model->char_loc, model->map, model->pcs[i]);
	}
	model->selY = model->pcs[model->cur_pc]->y;
	model->selX = model->pcs[model->cur_pc]->x;
	
	//Draw the loaded or blank screen
	ui->draw();
	refresh();

	//invalid key means a new input can be parsed.
	int invalid;
	//Exit means input should no longer be read and program will close after key is handled
	int exit = 0;
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
		
		switch(key){
			case 'w':
				if(model->pcs[model->cur_pc]->movement_map[yx_to_index(model->selY-1, model->selX)]){
					model->selY--;
				}
				break;
			case 's':
				if(model->pcs[model->cur_pc]->movement_map[yx_to_index(model->selY+1, model->selX)]){
					model->selY++;
				}
				break;
			case 'a':
				if(model->pcs[model->cur_pc]->movement_map[yx_to_index(model->selY, model->selX-1)]){
					model->selX--;
				}
				break;
			case 'd':
				if(model->pcs[model->cur_pc]->movement_map[yx_to_index(model->selY, model->selX+1)]){
					model->selX++;
				}
				break;
			case ' ':
				pc_move(model->pcs[model->cur_pc], model->selY, model->selX);
				model->selY = model->pcs[model->cur_pc]->y;
				model->selX = model->pcs[model->cur_pc]->x;
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
					case 'I': //ctrl-I and also TAB ('\t')
						model->cur_pc = (model->cur_pc + 1) % model->num_pcs;
						model->selY = model->pcs[model->cur_pc]->y;
						model->selX = model->pcs[model->cur_pc]->x;
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
	if(!fread(&version, sizeof(uint8_t), 1, input)){
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

