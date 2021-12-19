#include <SDL2/SDL.h>
#include "initSDL.hpp"
#include "drawText.hpp"
#include "unit.hpp"
#include "map.hpp"
#include "level.hpp"
#include "cursor.hpp"
#include "game.hpp"

Game::Game(const char *filename) {
	level.load(filename);
	sidebar.rebuild();
	oldX = 0;
	oldY = 0;
	cameraX = 0;
	cameraY = 0;
	cameraXV = 0;
	cameraYV = 0;
	quit = false;
	drag = false;
	btn = 0;
	oldCamX = 0;
	oldCamY = 0;
	mousedown = false;
}

void Game::update(int diff) {
	level.update(diff);

	int x, y;
	getMouseXY(&x, &y);

	cameraX += cameraXV*diff;
	cameraY += cameraYV*diff;

	if(mousedown) {
		int xd = x-oldX, yd = y-oldY;
		if(xd*xd + yd*yd > 12*12)
			drag = true;

		if(drag && (btn & SDL_BUTTON_RMASK)) {
			cameraX = oldCamX-xd;
			cameraY = oldCamY-yd;
		}
	}

	if(cameraX < 0)
		cameraX = 0;
	if(cameraX >= level.map.w*24-240)
		cameraX = level.map.w*24-240-1;
	if(cameraY < 0)
		cameraY = 0;
	if(cameraY >= level.map.h*24-240)
		cameraY = level.map.h*24-240-1;
}

SDL_Rect Game::getSelectRect() {
	int x, y;
	getMouseXY(&x, &y);

	SDL_Rect r = (SDL_Rect) {
		oldX+(int)oldCamX, oldY+(int)oldCamY,
		x-oldX + (int)(cameraX-oldCamX), y-oldY + (int)(cameraY-oldCamY)
	};

	if(r.w < 0) {
		r.w *= -1;
		r.x -= r.w;
	}
	if(r.h < 0) {
		r.h *= -1;
		r.y -= r.h;
	}

	return r;
}

void Game::drawSelectRect() {
	SDL_Rect r = getSelectRect();
	r.x -= cameraX;
	r.y -= cameraY;

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x88);
	SDL_RenderFillRect(renderer, &r);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawRect(renderer, &r);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
}

void Game::draw() {
	SDL_RenderClear(renderer);

	level.map.draw(-cameraX, -cameraY);
	
	for(int i = 0; i < selectedUnits.size(); i++)
		if(selectedUnits.at(i))
			selectedUnits.at(i)->drawUI_bottom(-cameraX, -cameraY);

	level.drawUnits(-cameraX, -cameraY);

	if(drag && mousedown && (btn & SDL_BUTTON_LMASK))
		drawSelectRect();

	for(int i = 0; i < selectedUnits.size(); i++)
		if(selectedUnits.at(i))
			selectedUnits.at(i)->drawUI_top(-cameraX, -cameraY);

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
	getMouseXY(&x, &y);

	int xd = x-oldX, yd = y-oldY;

	if(drag) {
		if(btn & SDL_BUTTON_LMASK) {
			SDL_Rect r = getSelectRect();

			selectedUnits.clear();

			for(int tx = r.x/24; tx <= r.x/24+r.w/24; tx++)
				for(int ty = r.y/24; ty <= r.y/24+r.h/24; ty++) {
					Unit *u = level.unitAt(tx, ty);
					if(u)
						selectedUnits.push_back(u);
				}
		}

		/*if(btn & SDL_BUTTON_RMASK) {
			cameraX = oldCamX+xd;
			cameraY = oldCamY+yd;
		}*/

		return;
	}

	if(x >= 240) {
		if(btn & SDL_BUTTON_LMASK)
			sidebar.click();
		return;
	}

	x = (x+cameraX)/24;
	y = (y+cameraY)/24;

	if(btn & SDL_BUTTON_LMASK) {
		selectedUnits.clear();
		Unit *u = level.unitAt(x, y);
		if(u)
			selectedUnits.push_back(u);
	}
	else if(btn & SDL_BUTTON_RMASK)
		for(int i = 0; i < selectedUnits.size(); i++)
			if(selectedUnits.at(i))
				selectedUnits.at(i)->target(x, y);
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
					btn = getMouseXY(&oldX, &oldY);
					oldCamX = cameraX;
					oldCamY = cameraY;
					drag = false;
					mousedown = true;
					break;
				case SDL_MOUSEBUTTONUP:
					click();
					mousedown = false;
					break;
			}

		int currentTime = SDL_GetTicks();
		int diff = currentTime-lastUpdate;
		lastUpdate = currentTime;

		update(diff);

		draw();
	}
}
