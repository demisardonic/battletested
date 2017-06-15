#include <ncurses.h>

#include <stdint.h>

#include "model.h"
#include "ui/page.h"
#include "ui/page_game.h"
#include "ui/ui.h"



GamePage::GamePage(UI *ui):Page(ui){
	
}

Model *GamePage::model(){
	return parent->get_model();
}

void GamePage::draw(){
	int i, j;
	
	attron(COLOR_PAIR(COLOR_DEFAULT));
	
	draw_boarder();
	
	//Draw the map
	for(i = 0; i < GAME_HEIGHT; i++){
		for (j = 0; j < GAME_WIDTH; j++){
			draw_char(i,j,model()->map[i*GAME_WIDTH+j]);
		}
	}
	
	attroff(COLOR_PAIR(COLOR_DEFAULT));
	
	if(model()->cur_pc != -1){
		//Draw the current selected pc move area
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			if((*model()->squad)[model()->cur_pc]->movement_map[i] > 0){
				if((*model()->squad)[model()->cur_pc]->movement_map[i]<=(*model()->squad)[model()->cur_pc]->speed){
					draw_char_color(i/GAME_WIDTH, i%GAME_WIDTH, model()->map[i], COLOR_DMOVE);
				} else {
					draw_char_color(i/GAME_WIDTH, i%GAME_WIDTH, model()->map[i], COLOR_MOVE);
				}
			}
		}
		
		//Draw the current selected tile
		draw_char_color(model()->moveY, model()->moveX, model()->map[model()->moveY * GAME_WIDTH + model()->moveX], COLOR_PC);
		
		//Draw all of the characters
		for(i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++){
			Character *ch = model()->char_loc[i];
			if(ch){
				draw_char_color(ch->y, ch->x, ch->c, COLOR_DEFAULT);
			}
		}
		
		//Draw the currently selected player
		draw_char_color((*model()->squad)[model()->cur_pc]->y, (*model()->squad)[model()->cur_pc]->x, (*model()->squad)[model()->cur_pc]->c, COLOR_PC);
		
		//Draw the selected player's info
		draw_cur_pc_info((*model()->squad)[model()->cur_pc]);
	}
}

void GamePage::input(){
	int key = getch();
		
	switch(key){
		/*case 'i':
			if(ui->mode == MODE_GAME){
				logger("Switching to Squad Selection UI.");
				ui->mode = MODE_SELECT_SQUAD;
			} else {
				logger("Switching to Game UI.");
				ui->mode = MODE_GAME;
			}
			break;*/
		case 'w':
			//if(ui->mode == MODE_GAME){
			if(model()->cur_pc != -1 && (*model()->squad)[model()->cur_pc]->movement_map[yx_to_index(model()->moveY-1, model()->moveX)]){
				model()->moveY--;
			}
			/*} else if(ui->mode == MODE_SELECT_SQUAD){
				if(model->selection > 0){
					model->selection--;
				}else{
					model->selection = model->num_pc_info-1;
				}
			}*/
			break;
		case 's':
			//if(ui->mode == MODE_GAME){
			if(model()->cur_pc != -1 && (*model()->squad)[model()->cur_pc]->movement_map[yx_to_index(model()->moveY+1, model()->moveX)]){
				model()->moveY++;
			}
			/*} else if(ui->mode == MODE_SELECT_SQUAD){
				if(model()->selection < model()->num_pc_info-1){
					model()->selection++;
				}else{
					model()->selection = 0;
				}
			}*/
			break;
		case 'a':
			if(model()->cur_pc != -1 && (*model()->squad)[model()->cur_pc]->movement_map[yx_to_index(model()->moveY, model()->moveX-1)]){
				model()->moveX--;
			}
			break;
		case 'd':
			if(model()->cur_pc != -1 && (*model()->squad)[model()->cur_pc]->movement_map[yx_to_index(model()->moveY, model()->moveX+1)]){
				model()->moveX++;
			}
			break;
		case ' ':
			//if(ui->mode == MODE_GAME){
				if(model()->cur_pc != -1){
					pc_move((*model()->squad)[model()->cur_pc], model()->moveY, model()->moveX);
					model()->moveY = (*model()->squad)[model()->cur_pc]->y;
					model()->moveX = (*model()->squad)[model()->cur_pc]->x;
				}
			/*} else if(ui->mode == MODE_SELECT_SQUAD){
				if(model->pc_info[model->selection]->in_squad && model->num_pcs > 0){
					model->pc_info[model->selection]->in_squad = 0;
					model->num_pcs--;
				}else if(!model->pc_info[model->selection]->in_squad && model->num_pcs < MAX_SQUAD_SIZE){
					model->pc_info[model->selection]->in_squad = 1;
					model->num_pcs++;
				}
			}*/
			break;
		/*case '\n':
			if(ui->mode == MODE_SELECT_SQUAD){
				logger("Setting new Squad Selection.");
				logger("Clearing current squad of size %d.", model->num_pcs);
				uint8_t i;
				for(i = 0; i < (*model->squad).size(); i++){
					model->char_loc[yx_to_index((*model->squad)[i]->y, (*model->squad)[i]->x)] = NULL;
					Character *temp = (*model->squad).back();
					model->squad->pop_back();
					delete temp;
				}
				model->num_pcs = 0;
				model->cur_pc = -1;
				int count = 0;
				for(i = 0; i < model->num_pc_info; i++){
					if(model->pc_info[i]->in_squad){
						model->squad->push_back(new Character(model->pc_info[i]));
						model->char_loc[yx_to_index((*model->squad)[count]->y, (*model->squad)[count]->x)] = (*model->squad)[count];
						update_valid_moves(model->char_loc, model->map, (*model->squad)[count]);
						model->cur_pc = count;
						model->moveY = (*model->squad)[model->cur_pc]->y; 
						model->moveX = (*model->squad)[model->cur_pc]->x;
						model->num_pcs++;
						count++;
					}
				}
				ui->mode = MODE_GAME;
			}
			break;*/
		case '\t':
			if(model()->cur_pc != -1){
				rotate_cur_pc();
			}
			break;
		case 'q':
			parent->change_page(new ExitPage(parent));
			break;
		
	}
}

void GamePage::enter(){
	clear();
}

void GamePage::exit(){
	delete this;
}

void GamePage::draw_char(int y, int x, uint8_t val){
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

void GamePage::draw_char_color(int y, int x, uint8_t val, int color){
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

void GamePage::draw_cur_pc_info(Character *pc){
	mvprintw(BOTTOM_BAR_1, 0, LINE_CLEAR);
	mvprintw(BOTTOM_BAR_1, 0, "Name:%s %s X:%d Y:%d", pc->info->f_name.c_str(), pc->info->l_name.c_str(), pc->x, pc->y);
	
	move(BOTTOM_BAR_2, 0);
	int i;
	for(i = 0; i < 7; i++){
		printw("S%d:%d ", i, pc->info->stats[i]);
	}
}

void GamePage::draw_boarder(){
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
