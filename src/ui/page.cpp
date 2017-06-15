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

ExitPage::ExitPage(UI *ui):BlankPage(ui){
	
}

void ExitPage::draw(){
	clear();
	int i,j,j2;
	i = 9; 
	j = 29; 
	j2 = 9;
	mvprintw(i,j,"             _____");
	mvprintw(i+1,j,"            /  ___|");
	mvprintw(i+2,j,"            \\ `--.   ___   ___   _   _   __ _");
	mvprintw(i+3,j,"             `--. \\ / _ \\ / _ \\ | | | | / _` |");
	mvprintw(i+4,j,"            /\\__/ /|  __/|  __/ | |_| || (_| |");
	mvprintw(i+5,j,"            \\____/  \\___| \\___|  \\__, | \\__,_|");
	mvprintw(i+6,j,"");
	mvprintw(i+6,j2,"                                              _       __/ |");
	mvprintw(i+7,j2,"                                             | |     |___/       ");
	mvprintw(i+8,j2," ___ _ __   __ _  ___ ___    ___ _____      _| |__   ___  _   _       ");
	mvprintw(i+9,j2,"/ __| '_ \\ / _` |/ __/ _ \\  / __/ _ \\ \\ /\\ / / '_ \\ / _ \\| | | |      ");
	mvprintw(i+10,j2,"\\__ \\ |_) | (_| | (_|  __/ | (_| (_) \\ V  V /| |_) | (_) | |_| |_ _ _ ");
	mvprintw(i+11,j2,"|___/ .__/ \\__,_|\\___\\___|  \\___\\___/ \\_/\\_/ |_.__/ \\___/ \\__, (_|_|_)");
	mvprintw(i+12,j2,"    | |                                                    __/ |      ");
	mvprintw(i+13,j2,"    |_|                                                   |___/       ");
}

void ExitPage::input(){
	getch();
	parent->close();
}