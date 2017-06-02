#include <ncurses.h>
#include <stdlib.h>

#include "character.h"
#include "logger.h"
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

static void draw_cur_pc_info(character_t *pc){
	mvprintw(BOTTOM_BAR_1, 0, LINE_CLEAR);
	mvprintw(BOTTOM_BAR_1, 0, "Name:%s %s X:%d Y:%d", pc->info->f_name, pc->info->l_name, pc->x, pc->y);
}

static void draw_boarder(){
	int i, j;
	//Draw the boarder
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

static void draw_game(){
	int i, j;
	
	attron(COLOR_PAIR(COLOR_DEFAULT));
	
	draw_boarder();
	
	//Draw the map
	for(i = 0; i < GAME_HEIGHT; i++){
		for (j = 0; j < GAME_WIDTH; j++){
			draw_char(i,j,ui->model->map[i*GAME_WIDTH+j]);
		}
	}
	
	attroff(COLOR_PAIR(COLOR_DEFAULT));
	
	if(ui->model->cur_pc != -1){
		//Draw the current selected pc move area
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			if(ui->model->squad[ui->model->cur_pc]->movement_map[i] > 0){
				if(ui->model->squad[ui->model->cur_pc]->movement_map[i]<=ui->model->squad[ui->model->cur_pc]->speed){
					draw_char_color(i/GAME_WIDTH, i%GAME_WIDTH, ui->model->map[i], COLOR_DMOVE);
				} else {
					draw_char_color(i/GAME_WIDTH, i%GAME_WIDTH, ui->model->map[i], COLOR_MOVE);
				}
			}
		}
		
		//Draw the current selected tile
		draw_char_color(ui->model->moveY, ui->model->moveX, ui->model->map[ui->model->moveY * GAME_WIDTH + ui->model->moveX], COLOR_PC);
		
		//Draw all of the characters
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			character_t *ch = ui->model->char_loc[i];
			if(ch){
				draw_char_color(ch->y, ch->x, ch->c, COLOR_DEFAULT);
			}
		}
		
		//Draw the currently selected player
		draw_char_color(ui->model->squad[ui->model->cur_pc]->y, ui->model->squad[ui->model->cur_pc]->x, ui->model->squad[ui->model->cur_pc]->c, COLOR_PC);
		
		//Draw the selected player's info
		draw_cur_pc_info(ui->model->squad[ui->model->cur_pc]);
	}
}

static void draw_select_squad(){
	int i;
	clear();
	for(i = 0; i < ui->model->num_pc_info; i++){
		if(ui->model->pc_info[i]){
			if(ui->model->pc_info[i]->in_squad){
				mvaddch(i+2,1, '*');
			}
			mvprintw(i+2,3, "%s %s", ui->model->pc_info[i]->f_name, ui->model->pc_info[i]->l_name);
		}
	}
	
	mvaddch(ui->model->selection+2, 2, '[');
	mvaddch(ui->model->selection+2, GAME_WIDTH, ']');
	draw_boarder();
}

static void ui_draw(){
	if(ui->mode == MODE_GAME) {
		//Draw the game ui
		draw_game();
	} else if(ui->mode == MODE_SELECT_SQUAD) {
		//Draw the squad selection
		draw_select_squad();
	} else {
		
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
	
	ui->mode = 1;
	
	ui->draw = ui_draw;
	ui->prompt = ui_prompt;
	ui->message = ui_message;
	return ui;
}

void free_ui(ui_t* ui){
	free(ui);
}

