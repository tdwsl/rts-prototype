#include <SDL2/SDL.h>
#include "initSDL.hpp"
#include "drawText.hpp"
#include "unit.hpp"
#include "map.hpp"
#include "level.hpp"
#include "loadTextures.hpp"
#include "game.hpp"

Game::Game(const char *filename) {
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
	level.drawUnits(-cameraX, -cameraY);

	drawText(0, 0, "Hello, world!");

	/*SDL_Rect src = (SDL_Rect){0, 0, 80, 240};
	SDL_Rect dst = (SDL_Rect){240, 0, 80, 240};
	SDL_RenderCopy(renderer, texture::ui, &src, &dst);*/
	sidebar.draw();

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
			}

		int currentTime = SDL_GetTicks();
		int diff = currentTime-lastUpdate;
		lastUpdate = currentTime;

		update(diff);

		draw();
	}
}
