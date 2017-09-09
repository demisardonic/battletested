#include <ncurses.h>

#include <stdint.h>

#include "logger.h"
#include "model.h"
#include "ui/page.h"
#include "ui/page_game.h"
#include "ui/page_squad.h"
#include "ui/ui.h"

SquadPage::SquadPage(UI *ui):Page(ui){
  selection = 0;
}

Model *SquadPage::model(){
  return parent->get_model();
}

void SquadPage::draw(){
  int i;
  clear();
  for(i = 0; i < model()->num_pc_info; i++){
    if(model()->pc_info[i]){
      if(model()->pc_info[i]->in_squad){
	mvaddch(i+2,1, '*');
      }
      mvprintw(i+2,3, "%s %s", model()->pc_info[i]->f_name.c_str(), model()->pc_info[i]->l_name.c_str());
      move(i+2, 23);
      int j;
      for(j = 0; j < 7; j++){
	printw("S%d:%d ", j, model()->pc_info[i]->stats[j]);
      }
    }
  }
  
  mvaddch(selection+2, 2, '[');
  mvaddch(selection+2, GAME_WIDTH, ']');
  int j;
  //Draw the boarder
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
}

void SquadPage::input(){
  int key = getch();
  
  switch(key){
  case 'w':
    if(selection > 0){
      selection--;
    }else{
      selection = model()->num_pc_info-1;
    }
    
    break;
  case 's':
    if(selection < model()->num_pc_info-1){
      selection++;
    }else{
      selection = 0;
    }
    break;
    
  case ' ':
    if(model()->pc_info[selection]->in_squad && model()->num_pcs > 0){
      model()->pc_info[selection]->in_squad = 0;
      model()->num_pcs--;
    }else if(!model()->pc_info[selection]->in_squad && model()->num_pcs < MAX_SQUAD_SIZE){
      model()->pc_info[selection]->in_squad = 1;
      model()->num_pcs++;
    }
    break;
  case '\n':
    logger("Setting new Squad Selection.");
    logger("Clearing current squad of size %d.", model()->num_pcs);
    uint8_t i;
    while((*model()->squad).size()){
      Character *temp = (*model()->squad).back();
      model()->char_loc[yx_to_index(temp->y, temp->x)] = NULL;
      model()->squad->pop_back();
      delete temp;
    }
    model()->num_pcs = 0;
    model()->cur_pc = -1;
    for(i = 0; i < model()->num_pc_info; i++){
      if(model()->pc_info[i]->in_squad){
	model()->squad->push_back(new Character(model()->pc_info[i]));
	model()->char_loc[yx_to_index((*model()->squad)[model()->num_pcs]->y, (*model()->squad)[model()->num_pcs]->x)] = (*model()->squad)[model()->num_pcs];
	update_valid_moves(model()->char_loc, model()->map, (*model()->squad)[model()->num_pcs]);
	model()->num_pcs++;
      }
    }
    logger("New squad has size %d.", (*model()->squad).size());
    model()->cur_pc = model()->num_pcs - 1;
    if(model()->num_pcs > 0){
      model()->moveY = (*model()->squad)[model()->cur_pc]->y;
      model()->moveX = (*model()->squad)[model()->cur_pc]->x;
    }
    parent->change_page(new GamePage(parent));
    break;
    
  }
}

void SquadPage::update(){
  
}

void SquadPage::enter(){
  clear();
}

void SquadPage::exit(){
  delete this;
}
