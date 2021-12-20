#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <vector>
#include "team.hpp"

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
	Team *team;
	int page;
	int cindex;
	void addCategory(const char *name, int icon);
	int actionAt(int x, int y);
public:
	Sidebar();
	void rebuild();
	void draw();
	void click();

	friend class Game;
};

#endif
