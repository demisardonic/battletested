#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "util.h"
#include "logger.h"

int draw_map(char map[]);

int main(int argc, char** args){
	char map[GAME_HEIGHT * GAME_WIDTH];
	int i;
	for(i = 0; i < GAME_WIDTH*GAME_HEIGHT; i++){
		map[i] = 2;
	}
	//init ncurses
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	draw_map(map);
	
	int valid = 0;
	int exit = 0;
	while(1){
		int key = getch();
		switch(key){
			case 'q':
				exit = 1;
				break;
			default:  
				break;
		}
		if(exit){
			break;
		}
		if(!valid){
			continue;
		}
	}

	//delete ncurses window
	endwin();
	return 0;
}

int draw_map(char map[]){
	mvprintw(0, 0, "TOP BAR PRINTING STUFF. TOP BAR PRINTING STUFF. TOP BAR PRINTING STUFF.");
	
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
	for(i = 0; i < GAME_HEIGHT; i++){
		for (j = 0; j < GAME_WIDTH; j++){
			move(2+i, 1+j);
			char val = map[i*GAME_WIDTH+j];
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
	/*
	for(i = 0; i < GAME_HEIGHT+2; i++){
		for (j = 0; j < GAME_WIDTH+2; j++){
			move(i+1,j);
			if(i == 0 && j == 0){
				addch(ACS_ULCORNER);
			}else if(i == 0 && j == GAME_WIDTH+1){
				addch(ACS_URCORNER);
			}else if(i == GAME_HEIGHT+1 && j == 0){
				addch(ACS_LLCORNER);
			}else if(i == GAME_HEIGHT+1 && j == GAME_WIDTH+1){
				addch(ACS_LRCORNER);
			}else if(i == 0 || i == GAME_HEIGHT+1){
				addch(ACS_HLINE);
			}else if(j == 0 || j == GAME_WIDTH+1){
				addch(ACS_VLINE);
			}else{
				char val = map[i*GAME_WIDTH+j];
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
	}*/

	mvprintw(BOTTOM_BAR_1, 0, "BOTTOM BAR PART 1 BOTTOM BAR PART 1 BOTTOM BAR PART 1 BOTTOM BAR PART 1");
	mvprintw(BOTTOM_BAR_2, 0, "BOTTOM BAR PART 2 BOTTOM BAR PART 2 BOTTOM BAR PART 2 BOTTOM BAR PART 2");
	refresh();
	return 1;
}