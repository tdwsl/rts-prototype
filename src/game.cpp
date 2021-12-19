#include <SDL2/SDL.h>
#include "initSDL.hpp"
#include "drawText.hpp"
#include "unit.hpp"
#include "map.hpp"
#include "level.hpp"
#include "cursor.hpp"
#include "game.hpp"

Game::Game(const char *filename) {
	unit = 0;
	level.load(filename);
	sidebar.rebuild();
}

void Game::update(int diff) {
	level.update(diff);

	cameraX += cameraXV*diff;
	cameraY += cameraYV*diff;
	if(cameraX < 0)
		cameraX = 0;
	if(cameraX >= level.map.w*24-240)
		cameraX = level.map.w*24-240-1;
	if(cameraY < 0)
		cameraY = 0;
	if(cameraY >= level.map.h*24-240)
		cameraY = level.map.h*24-240-1;
}

void Game::draw() {
	SDL_RenderClear(renderer);

	level.map.draw(-cameraX, -cameraY);
	
	if(unit)
		unit->drawUI_bottom(-cameraX, -cameraY);

	level.drawUnits(-cameraX, -cameraY);

	if(unit)
		unit->drawUI_top(-cameraX, -cameraY);

	sidebar.draw();
	drawCursor();

	updateDisplay();
}

void Game::keyDown(SDL_Keycode sym) {
	switch(sym) {
		case SDLK_UP:
			cameraYV = -0.1;
			break;
		case SDLK_DOWN:
			cameraYV = 0.1;
			break;
		case SDLK_LEFT:
			cameraXV = -0.1;
			break;
		case SDLK_RIGHT:
			cameraXV = 0.1;
			break;
	}
}

void Game::keyUp(SDL_Keycode sym) {
	switch(sym) {
		case SDLK_UP:
			if(cameraYV < 0)
				cameraYV = 0;
			break;
		case SDLK_DOWN:
			if(cameraYV > 0)
				cameraYV = 0;
			break;
		case SDLK_LEFT:
			if(cameraXV < 0)
				cameraXV = 0;
			break;
		case SDLK_RIGHT:
			if(cameraXV > 0)
				cameraXV = 0;
			break;
	}
}

void Game::click() {
	int x, y;
	Uint32 btn = getMouseXY(&x, &y);

	if(x >= 240) {
		sidebar.click();
		return;
	}

	x = (x+cameraX)/24;
	y = (y+cameraY)/24;
	if(btn & SDL_BUTTON_LMASK) {
		unit = level.unitAt(x, y);
	}
	else if(unit && (btn & SDL_BUTTON_RMASK))
		unit->target(x, y);
}

void Game::run() {
	int lastUpdate = SDL_GetTicks();

	while(!quit) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
			switch(ev.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					keyDown(ev.key.keysym.sym);
					break;
				case SDL_KEYUP:
					keyUp(ev.key.keysym.sym);
					break;
				case SDL_MOUSEBUTTONDOWN:
					click();
					break;
			}

		int currentTime = SDL_GetTicks();
		int diff = currentTime-lastUpdate;
		lastUpdate = currentTime;

		update(diff);

		draw();
	}
}
