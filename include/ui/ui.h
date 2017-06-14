#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "ui/page.h"

class UI{
	private:
		Page *curPage;
	public:
	UI();
	~UI();
	void change_page(Page *newPage);
	void draw();
};

#endif
