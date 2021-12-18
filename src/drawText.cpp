#include <SDL2/SDL.h>
#include "initSDL.hpp"
#include "loadTextures.hpp"
#include "drawText.hpp"

#define TABSZ 4

void drawText(int x, int y, const char *text) {
	int cx = 0, cy = 0;
	for(const char *c = text; *c; c++) {
		SDL_Rect src = (SDL_Rect) {
			(*c%FONT_W)*4, (*c/FONT_W)*6, 4, 6
		};
		SDL_Rect dst = (SDL_Rect){x+cx*4, y+cy*6, 4, 6};
		SDL_RenderCopy(renderer, texture::font, &src, &dst);

		cx++;
		if(*c == '\n') {
			cx = 0;
			cy++;
		}
		else if(*c == '\t')
			cx += ((TABSZ)-(cx%(TABSZ+1)));
	}
}
