#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "level.hpp"
#include "sidebar.hpp"
#include "unit.hpp"

class Game {
	float cameraX, cameraY;
	float cameraXV, cameraYV;
	int oldX, oldY;
	float oldCamX, oldCamY;
	Level level;
	bool quit;
	Sidebar sidebar;
	std::vector<Unit*> selectedUnits;
	Uint32 btn;
	bool drag;
	bool mousedown;

	void draw();
	void update(int diff);
	void keyDown(SDL_Keycode keysym);
	void keyUp(SDL_Keycode keysym);
	void click();
	SDL_Rect getSelectRect();
	void drawSelectRect();
public:
	Game(const char *filename);
	void run();
};

#endif
