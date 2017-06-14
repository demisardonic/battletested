#ifndef PAGE_H
#define PAGE_H

class Page{
	public:
		virtual void draw() = 0;
		virtual void input() = 0;
		virtual void enter() = 0;
		virtual void exit() = 0;
		virtual ~Page();
};

#endif