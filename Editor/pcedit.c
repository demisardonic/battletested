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
#define GAME_LINE_CLEAR "                                                                              "
#define NUM_FIELDS 9
#define COLOR_SELECTED 1

typedef struct character_info{
	char* f_name;
	char* l_name;
	uint8_t stats[7];
} character_info_t;

void draw_boarder();
void draw_selected(uint8_t selected, const char* fmt, ...);
void ui_prompt(char* input, const char* fmt, ...);
void ui_prompt_val(int* val, const char* fmt, ...);
int is_alphanumeric_char(char val);
int is_path_char(char val);
character_info_t *read_characters_from_file(const char* path, uint8_t *num_pc_info);
int save_characters_to_file(character_info_t *pcs, uint8_t len, const char* path);

int main(int argc, char** argv){
	
	character_info_t *player_info = NULL;
	uint8_t num_pc_info = 0;
	uint8_t selection = 0;
	int top = 0;
	uint8_t edit_section = 0;
	
	int i, j;
	
	//character arrays to store load and save paths.
	//"strings" initially set to "\0".
	char loadPath[BUFFER_SIZE];
	char savePath[BUFFER_SIZE];
	memset(loadPath, 0, BUFFER_SIZE);
	memset(savePath, 0, BUFFER_SIZE);
	
	if(argc > 1){
		int i;
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
		player_info = read_characters_from_file(loadPath, &num_pc_info);
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
	init_pair(COLOR_SELECTED, COLOR_BLACK, COLOR_WHITE);

	//Draw the border
	draw_boarder();
	refresh();
	
	//Valid key means a new input can be parsed.
	int valid;
	//Exit means input should no longer be read and program will close after key is handled
	int exit = 0;
	while(1){
		clear();
		for(i = top; i < num_pc_info + top; i++){
			move(i+2, 3);
			if(selection == i && edit_section == 1){
				attron(COLOR_PAIR(COLOR_SELECTED));
			}
			printw("%s ", player_info[i].f_name);
			if(selection == i && edit_section == 1){
				attroff(COLOR_PAIR(COLOR_SELECTED));
			}
			if(selection == i && edit_section == 2){
				attron(COLOR_PAIR(COLOR_SELECTED));
			}
			printw("%s ", player_info[i].l_name);
			if(selection == i && edit_section == 2){
				attroff(COLOR_PAIR(COLOR_SELECTED));
			}
			move(i+2, 23);
			for(j = 0; j < 7; j++){
				if(selection == i && edit_section == 3+j){
				attron(COLOR_PAIR(COLOR_SELECTED));
				}
				printw("S%d:%d ", j, player_info[i].stats[j]);
				if(selection == i && edit_section == 3+j){
					attroff(COLOR_PAIR(COLOR_SELECTED));
				}
			}
		}
		mvaddch(i+2, 3, '+');
		for(i = 0; i<GAME_HEIGHT; i++){
			mvaddch(i+2, 1, ' ');
			mvaddch(i+2, 2, ' ');
			mvaddch(i+2, GAME_WIDTH, ' ');
		}
		mvaddch(selection+2, 2, '[');
		mvaddch(selection+2, GAME_WIDTH, ']');
		if(edit_section){
			mvaddch(selection+2, 1, '*');
		}
		draw_boarder();
		
		valid = 1;
		//Pull keyboard input
		int key = getch();
		
		switch(key){
			case 'w':
			case KEY_UP:
				selection = selection > 0? selection-1 : num_pc_info;
				edit_section = 0;
				break;
			case 's':
			case KEY_DOWN:
				selection = selection < num_pc_info? selection+1 : 0;
				edit_section = 0;
				break;
			case 'a':
			case KEY_LEFT:
				if(edit_section){
					edit_section--;
					if(edit_section < 1){
						edit_section = NUM_FIELDS;
					}
				}
				break;
			case 'd':
			case KEY_RIGHT:
				if(edit_section){
					edit_section++;
					if(edit_section > NUM_FIELDS){
						edit_section = 1;
					}
				}
				break;
			case ' ':
				if(edit_section == 1){
					ui_prompt(player_info[selection].f_name, "First name :");
				}else if(edit_section == 2){
					ui_prompt(player_info[selection].l_name, "Last name :");
				}else if(edit_section > 2 &&  edit_section <= NUM_FIELDS){
					int val = 0;
					ui_prompt_val(&val, "Stat (255 max): ");
					while(val >= 256){
						ui_prompt_val(&val, "Stat (255 max): ");
					}
					player_info[selection].stats[edit_section-3] = (uint8_t) val;
				}
				break;
			case '\n':
				edit_section = edit_section ? 0 : 1;
				if(selection == num_pc_info){
					mvprintw(0,0, "Resize to %d", (num_pc_info + 1));
					refresh();

					character_info_t *newptr = (character_info_t *) realloc(player_info, (num_pc_info + 1) * sizeof(character_info_t));
					if(newptr){
						player_info = newptr;
						player_info[num_pc_info].f_name = (char *) malloc(sizeof(char) * 6);
						strcpy(player_info[num_pc_info].f_name, "first");
						player_info[num_pc_info].l_name = (char *) malloc(sizeof(char) * 5);
						strcpy(player_info[num_pc_info].l_name, "last");
						for(i = 0; i < 7; i++){
							player_info[num_pc_info].stats[i] = 0;
						}
					}
					num_pc_info++;
				}
				break;
			case '\t':
				if(edit_section){
					edit_section++;
					if(edit_section > NUM_FIELDS){
						edit_section = 1;
					}
				}
				break;
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

	if(savePath[0] != '\0' && player_info){
		save_characters_to_file(player_info, num_pc_info, savePath);
	}
	for(i = 0; i < num_pc_info; i++){
		free(player_info[i].l_name);
		free(player_info[i].f_name);
	}
	free(player_info);
	return 0;
}

//Draw decorate boarder around map
void draw_boarder(){
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
}


void ui_prompt(char* input, const char* fmt, ...){
	int textKey;
	int index = 0;
	int escape = 0;
	curs_set(1);
	va_list args;
	va_start(args, fmt);
	mvprintw(0, 0, LINE_CLEAR);
	mvprintw(0, 0, fmt, args);
	va_end(args);
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
			mvprintw(0, 0, fmt, args);
			printw("%s", input);
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

void ui_prompt_val(int* val, const char* fmt, ...){
	char input[BUFFER_SIZE];
	memset(input, 0, BUFFER_SIZE);
	int textKey;
	int index = 0;
	int escape = 0;
	curs_set(1);
	va_list args;
	va_start(args, fmt);
	mvprintw(0, 0, LINE_CLEAR);
	mvprintw(0, 0, fmt, args);
	va_end(args);
	textKey = getch();
	while(textKey != '\n'){
		if(textKey >= '0' && textKey <= '9'){
			addch(textKey);
			input[index] = textKey;
			index++;
			input[index] = '\0';
		}else if(textKey == KEY_BACKSPACE){
			index--;
			input[index] = '\0';
			mvprintw(0, 0, LINE_CLEAR);
			mvprintw(0, 0, fmt, args);
			printw("%s", input);
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
	*val = atoi(input);
}

int is_alphanumeric_char(char val){
	if(val >= 'a' && val <= 'z'){
		return 1;
	}
	if(val >= 'A' && val <= 'Z'){
		return 1;
	}
	if(val >= '0' && val <= '9'){
		return 1;
	}
	return 0;
}

int is_path_char(char val){
	if(val == '.' || val == '_' || val == '-' || val == '/'){
		return 1;
	}
	return is_alphanumeric_char(val);
}

//Output a character_info array
character_info_t *read_characters_from_file(const char* path, uint8_t *num_pc_info){
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
		if(!fread(num_pc_info, sizeof(uint8_t), 1, input)){
			fprintf(stderr, "No num_pc_info.\n");
			return NULL;
		}
		character_info_t *players = (character_info_t *) malloc(sizeof(character_info_t) * (*num_pc_info));
		int i;
		uint8_t buff8;
		for(i = 0; i < *num_pc_info; i++){
			
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
	
	fwrite(&len, 1, 1, outfile);
	
	int i;
	for(i = 0; i < len; i++){
		uint8_t len = strlen(pcs[i].f_name);
		fwrite(&len, 1, 1, outfile);
		fwrite(pcs[i].f_name, sizeof(char), len, outfile);
		
		len = strlen(pcs[i].l_name);
		fwrite(&len, 1, 1, outfile);
		fwrite(pcs[i].l_name, sizeof(char), len, outfile);
		
		int j;
		for(j = 0; j < 7; j++){
			fwrite(&pcs[i].stats[j], 1, 1, outfile);
		}
		
	}
	
	fclose(outfile);
	return 0;
}
