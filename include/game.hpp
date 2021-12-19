#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "level.hpp"
#include "sidebar.hpp"
#include "unit.hpp"

class Game {
	float cameraX=0, cameraY=0;
	float cameraXV=0, cameraYV=0;
	Level level;
	bool quit=false;
	Sidebar sidebar;
	Unit *unit;

	void draw();
	void update(int diff);
	void keyDown(SDL_Keycode keysym);
	void keyUp(SDL_Keycode keysym);
	void click();
public:
	Game(const char *filename);
	void run();
};

#endif
