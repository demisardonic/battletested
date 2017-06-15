#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "ui/page.h"

class UI{
	private:
		Page *curPage;
		Page *nextPage;
		bool shouldChange;
		bool shouldClose;
		void internal_change_page();
	public:
	UI();
	~UI();
	void change_page(Page *newPage);
	void draw();
	void input();
	void update();
	inline void close(){ shouldClose = true; };
	inline bool should_close() { return shouldClose; };
};

#endif
