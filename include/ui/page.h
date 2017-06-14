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

class AnyKeyPage : public Page{
	public:
		AnyKeyPage(UI *ui);
		AnyKeyPage(UI *ui, Page *next);
		virtual void draw() = 0;
		void input();
		virtual void enter() = 0;
		void exit();
		~AnyKeyPage(){};
	private:
		Page *nextPage;
};

class TitlePage : public AnyKeyPage{
	public:
		TitlePage(UI *ui);
	
		void draw();
		void enter();
		~TitlePage(){};
};

#endif