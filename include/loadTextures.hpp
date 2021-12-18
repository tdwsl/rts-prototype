#ifndef LOADTEXTURES_H
#define LOADTEXTURES_H

#include <SDL2/SDL.h>

#define TILESET_W 4
#define UNITS_W 8
#define FONT_W 32

namespace texture {
	extern SDL_Texture
		*tileset,
		*units,
		*font,
		*ui;
};

void loadTextures();
void freeTextures();

#endif
