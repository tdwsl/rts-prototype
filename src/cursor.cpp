#include <SDL2/SDL.h>
#include "initSDL.hpp"
#include "loadTextures.hpp"
#include "cursor.hpp"

void drawCursor() {
	int mx, my;
	getMouseXY(&mx, &my);

	SDL_Rect src = (SDL_Rect){160, 0, 12, 12};
	SDL_Rect dst = (SDL_Rect){mx, my, 12, 12};

	SDL_RenderCopy(renderer, texture::ui, &src, &dst);
}
