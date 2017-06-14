#include "ui/page.h"
#include "ui/ui.h"

UI::UI(){
	this->curPage = new TitlePage(this);
	this->curPage->enter();
}

UI::~UI(){
	if(this->curPage){
		this->curPage->exit();
	}
}

void UI::change_page(Page *newPage){
	this->curPage->exit();
	delete this->curPage;
	this->curPage = newPage;
	this->curPage->enter();
}

void UI::draw(){
	this->curPage->draw();
}