#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define FILE_HEADER "btlplr"
#define GAME_WIDTH 78
#define GAME_HEIGHT 19
#define TOP_BAR 0
#define BOTTOM_BAR_1 22
#define BOTTOM_BAR_2 23
#define BUFFER_SIZE (1<<8)
#define VERSION 1
#define LINE_CLEAR "                                                                                "

typedef struct character_info{
	char* f_name;
	char* l_name;
	uint8_t stats[7];
	uint8_t in_squad;
} character_info_t;

int draw_boarder();
character_info_t *read_characters_from_file(const char* path, uint8_t *num_char_info);
int save_characters_to_file(character_info_t *pcs, uint8_t len, const char* path);

int main(int argc, char** argv){
	
	character_info_t *player_info;
	uint8_t num_pc_info;
	
	//character arrays to store load and save paths.
	//"strings" initially set to "\0".
	char loadPath[BUFFER_SIZE];
	char savePath[BUFFER_SIZE];
	memset(loadPath, 0, BUFFER_SIZE);
	memset(savePath, 0, BUFFER_SIZE);
	
	if(argc > 1){
		int i;
		strcpy(savePath, argv[1]);
		strcpy(loadPath, argv[1]);
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
				}
			}
		}
	}
	//If the loadPath was not changed via commandline arguments.
	if(loadPath[0] != '\0'){
		player_info = read_characters_from_file("players.btp", &num_pc_info);
	}
	//If map cannot be read from file or no path is provided initialize blank map.
	if(!player_info){
		
	}
	//NCurses screen initialization
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	//Draw the border
	draw_boarder();
	refresh();
	
	//Valid key means a new input can be parsed.
	int valid;
	//Exit means input should no longer be read and program will close after key is handled
	int exit = 0;
	while(1){
		valid = 1;
		//Pull keyboard input
		int key = getch();
		
		switch(key){
			case 'q':
				exit = 1;
				break;
			default:
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
	}

	//delete ncurses window
	endwin();

	if(savePath[0] != '\0'){
		save_characters_to_file(player_info, num_pc_info, savePath);
	}
	
	return 0;
}

//Draw decorate boarder around map
int draw_boarder(){
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
	return 0;
}

//Output a character_info array
character_info_t *read_characters_from_file(const char* path, uint8_t *num_char_info){
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
		character_info_t *players = (character_info_t *) malloc(sizeof(character_info_t) * (*num_char_info));
		int i;
		uint8_t buff8;
		for(i = 0; i < *num_char_info; i++){
			
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
			players[i].f_name = f_name;
			
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
			players[i].l_name = l_name;
			
			int j;
			for(j=0; j < 7; j++){
				if(!fread(&buff8, sizeof(uint8_t), 1, input)){
					fprintf(stderr, "Failed to read stat %d\n", j);
					return NULL;
				}
				players[i].stats[j] = buff8;
			}
			
			
			players[i].in_squad = 0;
		}
		return players;
	}
	return NULL;
}

//Save the map array to file
int save_characters_to_file(character_info_t *pcs, uint8_t len, const char* path){
	FILE* outfile = fopen(path, "w");
	if(!outfile){
		fprintf(stderr, "Could not save file: %s\n", path);
		return 1;
	}
	fwrite(FILE_HEADER, 6, 1, outfile);

	uint8_t ver = VERSION;
	fwrite(&ver, 1, 1, outfile);
	
	//fwrite(pcs, sizeof(*pcs), len, outfile);
	
	fclose(outfile);
	return 0;
}
