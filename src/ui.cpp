#include <SDL2/SDL.h>
#include "initSDL.hpp"
#include "loadTextures.hpp"
#include "unit.hpp"
#include "drawText.hpp"
#include "ui.hpp"

enum {
	ACTION_NONE=0,
	ACTION_SELECTCATEGORY=10,
	ACTION_SELECTUNIT=20,
	ACTION_PREVPAGE=30,
	ACTION_NEXTPAGE,
};

Category::Category(const char *name, int icon) {
	Category::name = name;
	Category::icon = icon;
}

void Category::drawIcon(int x, int y) {
	int t = 9+icon;
	SDL_Rect src = (SDL_Rect){160+(t%6)*12, (t/6)*12, 12, 12};
	SDL_Rect dst = (SDL_Rect){x, y, 12, 12};
	SDL_RenderCopy(renderer, texture::ui, &src, &dst);
}

void Category::add(int type) {
	types.push_back(type);
}

int Category::at(int i) {
	return types.at(i);
}

int Category::size() {
	return (int)types.size();
}

void Sidebar::addCategory(const char *name, int icon) {
	Category c(name, icon);
	categories.push_back(c);
}

void Sidebar::rebuild() {
	categories.clear();
	addCategory("Buildings", 0);
	addCategory("Vehicles", 1);
	addCategory("Infantry", 2);
	categories.at(2).add(UNITTYPE_SOLDIER);

	if(cindex >= categories.size())
		cindex = 0;
	if(page > categories.at(cindex).size()/6) {
		page = categories.at(cindex).size()/6-1;
		if(page < 0)
			page = 0;
	}
}

int Sidebar::actionAt(int x, int y) {
	if(x >= 16 && y >= 88 && x < 76 && y < 128) {
		int tx = (x-16)/20;
		int ty = (y-88)/20;
		int t = ty*3+tx;
		if(t >= categories.at(cindex).size())
			return ACTION_NONE;
		return ACTION_SELECTUNIT+t;
	}

	if(x >= 32 && y >= 132 && x < 56 && y < 144) {
		if(page <= 0)
			return ACTION_NONE;
		return ACTION_PREVPAGE;
	}

	if(x >= 56 && y >= 132 && x < 80 && y < 144) {
		if(page >= categories.size()/6)
			return ACTION_NONE;
		return ACTION_NEXTPAGE;
	}

	return ACTION_NONE;
}

void Sidebar::draw() {
	SDL_Rect src = (SDL_Rect){0, 0, 80, 240};
	SDL_Rect dst = (SDL_Rect){240, 0, 80, 240};
	SDL_RenderCopy(renderer, texture::ui, &src, &dst);

	int mx, my;
	getMouseXY(&mx, &my);
	mx -= 240;
	if(mx < 0)
		return;

	const char *hovertext;
	int a = actionAt(mx, my);
	switch(a) {
		default:
			hovertext = "Hello!";
			break;
		case ACTION_PREVPAGE:
			hovertext = "previous";
			break;
		case ACTION_NEXTPAGE:
			hovertext = "next";
			break;
		case ACTION_SELECTCATEGORY:
			hovertext = categories.at(
					a-ACTION_SELECTCATEGORY).name;
			break;
		case ACTION_SELECTUNIT:
			hovertext = unitNames[a-ACTION_SELECTUNIT];
			break;
	}
	drawText(mx+240+12, my, hovertext);
}

void Sidebar::click() {
	int mx, my;
	getMouseXY(&mx, &my);
	mx -= 240;

	switch(actionAt(mx, my)) {
		default:
			break;
		case ACTION_PREVPAGE:
			page--;
			break;
		case ACTION_NEXTPAGE:
			page++;
			break;
	}
}
