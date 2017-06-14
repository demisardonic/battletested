#include <ncurses.h>

#include "ui/page.h"
#include "ui/ui.h"

Page::Page(UI *ui){
	this->parent = ui;
}

AnyKeyPage::AnyKeyPage(UI *ui):Page(ui){
	
}

AnyKeyPage::AnyKeyPage(UI *ui, Page *next):Page(ui){
	
}

void AnyKeyPage::input(){
	
}

void AnyKeyPage::exit(){
	delete this;
}

TitlePage::TitlePage(UI *ui):AnyKeyPage(ui){
	
}

void TitlePage::draw(){
	mvprintw(0,0,"test");
}

void TitlePage::enter(){
	
}