#ifndef PAGE_H
#define PAGE_H

class UI;

class Page{
	public:
		Page(UI *ui);
	
		virtual void draw() = 0;
		virtual void input() = 0;
		virtual void enter() = 0;
		virtual void exit() = 0;
		virtual ~Page(){};
	private:
		UI *parent;
};

class TitlePage : public Page{
	public:
		TitlePage(UI *ui);
	
		void draw();
		void input();
		void enter();
		void exit();
		~TitlePage();
};

#endif