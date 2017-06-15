#ifndef PAGE_GAME_H
#define PAGE_GAME_H

#include "model.h"
#include "ui/page.h"
#include "ui/ui.h"

class GamePage : public Page{
	public:
		GamePage(UI *ui);
		void draw();
		void input();
		void enter();
		void exit();
	private:
		Model *model();
};

#endif