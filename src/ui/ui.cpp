#include "ui/page.h"
#include "ui/ui.h"

UI::UI(){
	this->curPage = new TitlePage(this);
	this->curPage->enter();
	this->shouldClose = false;
	this->shouldChange = false;
}

UI::~UI(){
	this->curPage->exit();
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