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
	
}

void GamePage::input(){
	
}

void GamePage::enter(){
	
}

void GamePage::exit(){
	
}