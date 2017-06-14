#include <ncurses.h>

#include "ui/page.h"
#include "ui/ui.h"

Page::Page(UI *ui){
	this->parent = ui;
}

TitlePage::TitlePage(UI *ui):Page(ui){
	
}

void TitlePage::draw(){
	mvprintw(0,0,"test");
}

void TitlePage::input(){
	
}

void TitlePage::enter(){
	
}

void TitlePage::exit(){
	delete this;
}

TitlePage::~TitlePage(){
	
}