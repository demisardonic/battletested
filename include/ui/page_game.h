#ifndef PAGE_GAME_H
#define PAGE_GAME_H

#include <stdint.h>

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
		int rotate_cur_pc();
	private:
		Model *model();
		void draw_char(int y, int x, uint8_t val);
		void draw_char_color(int y, int x, uint8_t val, int color);
		void draw_cur_pc_info(Character *pc);
		void draw_boarder();
		int moveX;
		int moveY;
		int cur_pc;
};

#endif