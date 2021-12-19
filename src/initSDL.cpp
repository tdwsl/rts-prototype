#include <SDL2/SDL.h>
#include <cassert>
#include "initSDL.hpp"

SDL_Renderer *renderer = NULL;
SDL_Window *win = NULL;

SDL_Texture *scrbuf = NULL;

SDL_Texture *loadTexture(const char *filename) {
	SDL_Surface *surf;
	SDL_Texture *tex;

	assert(surf = SDL_LoadBMP(filename));
	SDL_SetColorKey(surf, SDL_TRUE,
		SDL_MapRGB(surf->format, 0xff, 0, 0xff));

	assert(tex = SDL_CreateTextureFromSurface(renderer, surf));
	SDL_FreeSurface(surf);
	return tex;
}

void initSDL() {
	assert(SDL_Init(SDL_INIT_EVERYTHING) >= 0);

	int w = 640, h = 480;

	SDL_DisplayMode dm;
	SDL_GetDesktopDisplayMode(0, &dm);
	if(dm.w < w)
		w = dm.w;
	if(dm.h < h)
		h = dm.h;

	assert(win = SDL_CreateWindow("Stipula RTS",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h, SDL_WINDOW_RESIZABLE));

	assert(renderer = SDL_CreateRenderer(win, -1,
		SDL_RENDERER_SOFTWARE));
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);

	assert(scrbuf = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, 320, 240));
	SDL_SetRenderTarget(renderer, scrbuf);

	SDL_SetCursor(SDL_DISABLE);
}

void endSDL() {
	SDL_DestroyTexture(scrbuf);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
}

SDL_Rect getScreenRect() {
	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	float xs = (float)w / 320.0;
	float ys = (float)h / 240.0;
	float scale = (ys <= xs) ? ys : xs;

	SDL_Rect r;
	r.w = 320.0*scale;
	r.h = 240.0*scale;
	r.x = w/2 - r.w/2;
	r.y = h/2 - r.h/2;
	return r;
}

void getMouseXY(int *x, int *y) {
	SDL_GetMouseState(x, y);
	SDL_Rect r = getScreenRect();

	*x -= r.x;
	*y -= r.y;
	*x /= r.w / 320.0;
	*y /= r.h / 240.0;
}

void updateDisplay() {
	SDL_SetRenderTarget(renderer, 0);
	SDL_RenderClear(renderer);

	SDL_Rect r = getScreenRect();
	SDL_RenderCopy(renderer, scrbuf, 0, &r);

	SDL_RenderPresent(renderer);
	SDL_SetRenderTarget(renderer, scrbuf);
}
