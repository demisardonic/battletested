#ifndef PAGE_SQUAD_H
#define PAGE_SQUAD_H

#include <stdint.h>

#include "model.h"
#include "ui/page.h"
#include "ui/ui.h"

class SquadPage : public Page{
	public:
		SquadPage(UI *ui);
		void draw();
		void input();
		void update();
		void enter();
		void exit();
	private:
		Model *model();
		int selection;
};

int rotate_cur_pc();

#endif
