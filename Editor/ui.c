#include <ncurses.h>
#include <stdlib.h>

#include "ui.h"
#include "util.h"

ui_t *ui;

//Draw entire map
void draw_map(uint8_t map[]){
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
}

//Draw colored selection box
void draw_selection(uint8_t map[], int selectedX, int selectedY, int selectedWidth, int selectedHeight){
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
}

//Print two lines of editor information (current x, y, width, height, and tile)
void draw_info(int selectedX, int selectedY, int selectedWidth, int selectedHeight){
	mvprintw(BOTTOM_BAR_1, 0, LINE_CLEAR);
	mvprintw(BOTTOM_BAR_1, 0, "Current X: %d, Current Y: %d", selectedX, selectedY);
	mvprintw(BOTTOM_BAR_2, 0, LINE_CLEAR);
	mvprintw(BOTTOM_BAR_2, 0, "Current Width: %d, Current Height: %d", selectedWidth, selectedHeight);
}

void ui_draw(){
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
	
	draw_map(ui->map);
	draw_selection(ui->map, ui->selectedX, ui->selectedY, ui->selectedWidth, ui->selectedHeight);
	draw_info(ui->selectedX, ui->selectedY, ui->selectedWidth, ui->selectedHeight);
	
}

void ui_message(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	mvprintw(0, 0, LINE_CLEAR);
	mvprintw(0, 0, fmt, args);
	va_end(args);
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

ui_t *init_ui(){
	ui = malloc(sizeof(ui_t));
	ui->map = malloc(GAME_HEIGHT * GAME_WIDTH);
	
	ui->selectedX = 0;
	ui->selectedY = 0;
	ui->selectedWidth = 1;
	ui->selectedHeight = 1;
	
	ui->draw = ui_draw;
	ui->prompt = ui_prompt;
	ui->message = ui_message;
	return ui;
}

void free_ui(ui_t* ui){
	free(ui->map);
	free(ui);
}

