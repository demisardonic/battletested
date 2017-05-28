#include <ncurses.h>
#include <stdlib.h>

#include "ui.h"
#include "util.h"



static void ui_draw(){
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
	ui_t *ui = malloc(sizeof(ui_t));
	ui->draw = ui_draw;
	ui->map = malloc(GAME_HEIGHT * GAME_WIDTH);
	return ui;
}

void free_ui(ui_t* ui){
	free(ui->map);
	free(ui);
}

