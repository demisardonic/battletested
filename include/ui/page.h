#ifndef PAGE_H
#define PAGE_H

class UI;

class Page{
	public:
		UI *parent;

		Page(UI *ui);

		virtual void draw() = 0;
		virtual void input() = 0;
		virtual void update() = 0;
		virtual void enter() = 0;
		virtual void exit() = 0;
		virtual ~Page(){};
};

class AnyKeyPage : public Page{
	public:
		AnyKeyPage(UI *ui);
		AnyKeyPage(UI *ui, Page *next);
		virtual void draw() = 0;
		void input();
		void update();
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

class BlankPage : public Page{
	public:
		BlankPage(UI *ui);

		void draw();
		void input();
		void update();
		void enter(){};
		void exit(){delete this;};
		~BlankPage(){};
};

class ExitPage : public BlankPage{
	public:
		ExitPage(UI *ui);
		void draw();
		void input();
};

#endif
