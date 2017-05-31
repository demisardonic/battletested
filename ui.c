#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>

#include "character.h"
#include "ui.h"
#include "util.h"

ui_t *ui;

static void draw_char(int y, int x, uint8_t val){
	move(2+y, 1+x);
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

static void draw_char_color(int y, int x, uint8_t val, int color){
	attron(COLOR_PAIR(color));
	move(2+y, 1+x);
	if(val == 0){
		addch(FLOOR);
	}else if (val == 1){
		addch(HALF_COVER);
	}else if(val == 2){
		addch(FULL_COVER);
	}else{
		addch(val);	
	}
	attroff(COLOR_PAIR(color));
}

//Draw entire map
static void draw_map(uint8_t map[]){
	attron(COLOR_PAIR(COLOR_DEFAULT));
	int i, j;
	for(i = 0; i < GAME_HEIGHT; i++){
		for (j = 0; j < GAME_WIDTH; j++){
			draw_char(i,j,map[i*GAME_WIDTH+j]);
		}
	}
	attroff(COLOR_PAIR(COLOR_DEFAULT));
}

static void draw_player(character_t *pc){
	draw_char_color(pc->y, pc->x, pc->c, pc->color);
}

static void draw_player_move(uint8_t *map, character_t *player){
	int i;
	for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
		if(player->movement_map[i] > 0){
			if(player->movement_map[i]<=player->speed){
				draw_char_color(i/GAME_WIDTH, i%GAME_WIDTH, map[i], COLOR_DMOVE);
			} else {
				draw_char_color(i/GAME_WIDTH, i%GAME_WIDTH, map[i], COLOR_MOVE);
			}
		}
	}
	draw_char_color(ui->model->selY, ui->model->selX, map[ui->model->selY * GAME_WIDTH + ui->model->selX], COLOR_PC);
}

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
	draw_map(ui->model->map);
	draw_player_move(ui->model->map, ui->model->pcs[ui->model->cur_pc]);
	//draw_player(ui->model->player);
	for(i = 0; i < ui->model->num_pcs; i++){
		draw_player(ui->model->pcs[i]);
	}
	
}

static void ui_message(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	mvprintw(0, 0, LINE_CLEAR);
	mvprintw(0, 0, fmt, args);
	va_end(args);
}

static void ui_prompt(char* input, const char* fmt, ...){
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

ui_t *init_ui(const model_t *model){
	ui = (ui_t *)malloc(sizeof(ui_t));
	ui->model = model;
	
	ui->draw = ui_draw;
	ui->prompt = ui_prompt;
	ui->message = ui_message;
	return ui;
}

void free_ui(ui_t* ui){
	free(ui);
}

