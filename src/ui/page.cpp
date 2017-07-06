#include <ncurses.h>

#include "ui/page.h"
#include "ui/page_game.h"
#include "ui/ui.h"

Page::Page(UI *ui){
	this->parent = ui;
}

AnyKeyPage::AnyKeyPage(UI *ui, Page *next):Page(ui){
	nextPage = next;
}

void AnyKeyPage::input(){
	getch();
	this->parent->change_page(nextPage);
}

void AnyKeyPage::exit(){
	delete this;
}

void AnyKeyPage::update(){

}

TitlePage::TitlePage(UI *ui):AnyKeyPage(ui, new GamePage(ui)){

}

void TitlePage::draw(){
	mvprintw(17,9," _             _    _    _        _____             _              _ ");
	mvprintw(18,9,"| |           | |  | |  | |      |_   _|           | |            | |");
	mvprintw(19,9,"| |__    __ _ | |_ | |_ | |  ___   | |    ___  ___ | |_   ___   __| |");
	mvprintw(20,9,"| '_ \\  / _` || __|| __|| | / _ \\  | |   / _ \\/ __|| __| / _ \\ / _` |");
	mvprintw(21,9,"| |_) || (_| || |_ | |_ | ||  __/  | |  |  __/\\__ \\| |_ |  __/| (_| |");
	mvprintw(22,9,"|_.__/  \\__,_| \\__| \\__||_| \\___|  \\_/   \\___||___/ \\__| \\___| \\__,_|");
}

void TitlePage::enter(){
}

BlankPage::BlankPage(UI *ui):Page(ui){

}

void BlankPage::draw(){
	clear();
}

void BlankPage::input(){
	getch();
}

void BlankPage::update(){
	
}
