#include "ui/page.h"
#include "ui/ui.h"

UI::UI(){
	
}

UI::~UI(){
	this->curPage->exit();
}

void UI::change_page(Page *newPage){
	this->curPage->exit();
	delete this->curPage;
	this->curPage = newPage;
	this->curPage->enter();
}