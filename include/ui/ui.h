#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

class UI{
	private:
		Page *curPage;
	public:
	UI();
	~UI();
	void change_page(Page *newPage);
};

#endif
