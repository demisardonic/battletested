#include <vector>

#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "character.h"
#include "logger.h"
#include "model.h"
#include "ui.h"
#include "ui/ui.h"
#include "util.h"

int read_map_from_file(const char* path, uint8_t *map);
int save_map_to_file(uint8_t *map, const char* path);
Character_Info **read_characters_from_file(const char* path, int *num_char_len);

int main(int argc, char** argv){
	srand(time(NULL));
	int i;
	
	//character arrays to store load and save paths.
	//"strings" initially set to "\0".
	char loadPath[BUFFER_SIZE];
	memset(loadPath, 0, BUFFER_SIZE);
	
	uint32_t seed = 0;

	//Read commandline arguments if they exist
	if(argc > 1){
		for(i = 1; i < argc; i++){
			if(argv[i][0] == '-'){
				if(!strcmp("--load", argv[i]) || (strlen(argv[i]) == 2 && argv[i][1] == 'l')){ //Load argument
					if(i+1 < argc){
						strcpy(loadPath, argv[++i]);
					}else{
						fprintf(stderr, "Incorrect usage of --load.");
						return 1;
					}
				}else if(!strcmp("--seed", argv[i])){ //Seed argument
					if(i+1 < argc){
						seed = atoi(argv[++i]);
					}else{
						fprintf(stderr, "Incorrect usage of --load.");
						return 1;
					}
				}else if(!strcmp("--nolog", argv[i])){ //Seed argument
					no_log();
				}
			}
		}
	}
	
	if(seed){
		srand(seed);
	}else{
		srand(time(NULL));
	}
	
	logger("Initializing model.");
	//Initialize the model
	Model *model = new Model;
	
	//Load the players.btp file
	int num_pc_info = 0;
	logger("Reading player info.");
	Character_Info **player_info = read_characters_from_file("players.btp", &num_pc_info);
	logger("Read %d players.", num_pc_info);
	model->num_pc_info = num_pc_info;
	model->pc_info = player_info;
	
	logger("Initializing map from \"%s\".", loadPath);
	//Attempt to read the map file and store it within the model
	uint8_t *map = (uint8_t *)malloc(GAME_HEIGHT * GAME_WIDTH);
	model->map = map;
	if(read_map_from_file(loadPath, model->map)){
		logger("Failed to read map generating blank map. ");
		//If map cannot be read from file initialize blank map.
		int i;
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			model->map[i] = 0;
		}
	}
	
	logger("Initializing pc array.");
	//Create an array of player character pointers stored in the model
	model->num_pcs = 0;
	
	logger("Initializing ui.");
	UI *ui = new UI(model);
	
	while(1){
		//Redraw the ui
		//ui->draw();
		//refresh();
		ui->draw();
		ui->input();
		ui->update();
		if(ui->should_close()){
			break;
		}
		
	}
	//delete ncurses window
	
	
	delete model;
	delete ui;
	
	logger("Exiting Sucessfully");
	return 0;
}

//Output map array generated from reading the given file path
int read_map_from_file(const char* path, uint8_t *map){
	//If the path is empty return
	if(!path[0]){
		return 1;
	}
	
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
Character_Info **read_characters_from_file(const char* path, int *num_char_info){
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
		Character_Info **players = new Character_Info*[*num_char_info];
		int i;
		uint8_t buff8;
		for(i = 0; i < *num_char_info; i++){
			
			players[i] = new Character_Info;
			
			if(!fread(&buff8, sizeof(uint8_t), 1, input)){
				fprintf(stderr, "Failed to read Name buff8\n");
				return NULL;
			}
			
			char *f_name = (char *) malloc(sizeof(char) * (buff8 + 1));
			
			if(!fread(f_name, sizeof(char), buff8, input)){
				fprintf(stderr, "Failed to read Name buff8\n");
				return NULL;
			}
			f_name[buff8] = '\0';
			players[i]->f_name = f_name;
			free(f_name);
			
			if(!fread(&buff8, sizeof(uint8_t), 1, input)){
				fprintf(stderr, "Failed to read Name buff8\n");
				return NULL;
			}
			
			char *l_name = (char *) malloc(sizeof(char) * (buff8 + 1));
			
			if(!fread(l_name, sizeof(char), buff8, input)){
				fprintf(stderr, "Failed to read Name buff8\n");
				return NULL;
			}
			l_name[buff8] = '\0';
			players[i]->l_name = l_name;
			free(l_name);
			
			int j;
			for(j=0; j < 7; j++){
				if(!fread(&buff8, sizeof(uint8_t), 1, input)){
					fprintf(stderr, "Failed to read stat %d\n", j);
					return NULL;
				}
				players[i]->stats[j] = buff8;
			}
			
			
			players[i]->in_squad = 0;
		}
		return players;
	}
	return NULL;
}