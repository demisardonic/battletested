#include <ncurses.h>

#include "logger.h"
#include "model.h"
#include "ui/page.h"
#include "ui/ui.h"
#include "util.h"

UI::UI(Model* model){
	this->model = model;
	this->curPage = new TitlePage(this);
	this->curPage->enter();
	this->shouldClose = false;
	this->shouldChange = false;
	
	logger("Initializing ncurses.");
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	init_color_pairs();
}

UI::~UI(){
	this->curPage->exit();
	logger("Terminating ncurses.");
	endwin();
}

void UI::change_page(Page *newPage){
	nextPage = newPage;
	this->shouldChange = true;
}

void UI::draw(){
	this->curPage->draw();
}

void UI::input(){
	this->curPage->input();
}

void UI::update(){
	if(this->shouldChange){
		internal_change_page();
		this->shouldChange = false;
	}
}

void UI::internal_change_page(){
	this->curPage->exit();
	this->curPage = nextPage;
	this->curPage->enter();
}