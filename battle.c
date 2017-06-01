#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "character.h"
#include "model.h"
#include "ui.h"
#include "util.h"

int read_map_from_file(const char* path, uint8_t *map);
int save_map_to_file(uint8_t *map, const char* path);
character_info_t **read_characters_from_file(const char* path, int *num_char_len);

int main(int argc, char** argv){
	srand(time(NULL));
	int i;
	
	int num_char_len = 0;
	character_info_t **player_info = read_characters_from_file("players.btp", &num_char_len);
	
	
	
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
	model->num_pcs = num_char_len;
	model->pcs = (character_t **) malloc(sizeof(character_t *) * model->num_pcs);
	for(i = 0; i < model->num_pcs; i++){
		model->pcs[i] = init_character(player_info[i]);
		model->char_loc[yx_to_index(model->pcs[i]->y, model->pcs[i]->x)] = model->pcs[i];
	}
	ui_t *ui = init_ui(model);
	
	free(player_info);
	
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
	
	for(i = 0; i < model->num_pcs; i++){
		update_valid_moves(model->char_loc, model->map, model->pcs[i]);
	}
	model->moveY = model->pcs[model->cur_pc]->y;
	model->moveX = model->pcs[model->cur_pc]->x;
	
	//Draw the loaded or blank screen
	ui->draw();
	refresh();

	//invalid key means a new input can be parsed.
	int invalid;
	//Exit means input should no longer be read and program will close after key is handled
	int exit;
	
	while(1){
		//Redraw the ui
		ui->draw();
		refresh();
		
		invalid = 0;
		exit = 0;
		
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
				if(model->pcs[model->cur_pc]->movement_map[yx_to_index(model->moveY-1, model->moveX)]){
					model->moveY--;
				}
				invalid = 1;
				break;
			case 's':
				if(model->pcs[model->cur_pc]->movement_map[yx_to_index(model->moveY+1, model->moveX)]){
					model->moveY++;
				}
				invalid = 1;
				break;
			case 'a':
				if(model->pcs[model->cur_pc]->movement_map[yx_to_index(model->moveY, model->moveX-1)]){
					model->moveX--;
				}
				invalid = 1;
				break;
			case 'd':
				if(model->pcs[model->cur_pc]->movement_map[yx_to_index(model->moveY, model->moveX+1)]){
					model->moveX++;
				}
				invalid = 1;
				break;
			case ' ':
				pc_move(model->pcs[model->cur_pc], model->moveY, model->moveX);
				model->moveY = model->pcs[model->cur_pc]->y;
				model->moveX = model->pcs[model->cur_pc]->x;
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
						model->moveY = model->pcs[model->cur_pc]->y;
						model->moveX = model->pcs[model->cur_pc]->x;
						invalid = 1;
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
		
		mvprintw(0,0,"%d\n", model->pcs[model->cur_pc]->turns);
		
		//Read new input
		if(invalid){
			continue;
		}
		int turnless = 0;
		while(model->pcs[model->cur_pc]->turns <= 0){
			model->cur_pc = (model->cur_pc + 1) % model->num_pcs;
			model->moveY = model->pcs[model->cur_pc]->y;
			model->moveX = model->pcs[model->cur_pc]->x;
			turnless++;
			//If all pcs have no more moves, exit.
			if(turnless >= model->num_pcs){
				exit = 1;
				break;
			}
		}
		
		//break from input gathering while loop
		if(exit){
			break;
		}
		
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

	if(strcmp(buffer, "btlmap")){
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

//Output a character_info array
character_info_t **read_characters_from_file(const char* path, int *num_char_info){
	FILE *input = fopen(path, "r");
	if(!input){
		fprintf(stderr, "File %s does not exist.\n", path);
		return NULL;
	}

 	char buffer[BUFFER_SIZE];
	if(fread(buffer, sizeof(char), 6, input) != 6){
		fprintf(stderr, "Failed to read header.\n");
		return NULL;
	}
	buffer[6] = '\0';

	if(strcmp(buffer, "btlplr")){
		fprintf(stderr, "Header did not match.\n");
		return NULL;
	}

	uint8_t version;
	if(!fread(&version, sizeof(uint8_t), 1, input)){
		fprintf(stderr, "Ininvalid file.\n");
		return NULL;
	}
	
	if(version == 1){
		
		if(!fread(num_char_info, sizeof(uint8_t), 1, input)){
			fprintf(stderr, "No num_char_info.\n");
			return NULL;
		}
		character_info_t **players = (character_info_t **) malloc(sizeof(character_info_t *) * (*num_char_info));
		int i;
		int length;
		for(i = 0; i < *num_char_info; i++){
			
			players[i] = (character_info_t *) malloc(sizeof(character_info_t));
			
			if(!fread(&length, sizeof(uint32_t), 1, input)){
				fprintf(stderr, "Failed to read Name Length\n");
				return NULL;
			}
			
			char *f_name = (char *) malloc(sizeof(char) * (length + 1));
			
			if(!fread(f_name, sizeof(char), length, input)){
				fprintf(stderr, "Failed to read Name Length\n");
				return NULL;
			}
			f_name[length] = '\0';
			players[i]->f_name = f_name;
			printf("name: %s\n", players[i]->f_name);
			
			if(!fread(&length, sizeof(uint32_t), 1, input)){
				fprintf(stderr, "Failed to read Name Length\n");
				return NULL;
			}
			
			char *l_name = (char *) malloc(sizeof(char) * (length + 1));
			
			if(!fread(l_name, sizeof(char), length, input)){
				fprintf(stderr, "Failed to read Name Length\n");
				return NULL;
			}
			l_name[length] = '\0';
			players[i]->l_name = l_name;
			printf("name: %s\n", players[i]->l_name);
		}
		return players;
	}
	return NULL;
}