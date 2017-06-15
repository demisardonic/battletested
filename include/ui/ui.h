#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "model.h"
#include "ui/page.h"

class UI{
	private:
		Model *model;
		Page *curPage;
		Page *nextPage;
		bool shouldChange;
		bool shouldClose;
		void internal_change_page();
	public:
	UI(Model *model);
	~UI();
	void change_page(Page *newPage);
	void draw();
	void input();
	void update();
	inline void close(){ shouldClose = true; };
	inline bool should_close() { return shouldClose; };
	inline Model *get_model() { return model; };
};

#endif
