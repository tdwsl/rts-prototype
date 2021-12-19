#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <vector>

class Category {
	std::vector<int> types;
	int icon;
public:
	const char *name;
	Category(const char *name, int icon);
	void add(int type);
	void drawIcon(int x, int y);
	int size();
	int at(int i);
};

class Sidebar {
	std::vector<Category> categories;
	int page;
	int cindex;
	void addCategory(const char *name, int icon);
	int actionAt(int x, int y);
public:
	Sidebar();
	void rebuild();
	void draw();
	void click();
};

#endif
