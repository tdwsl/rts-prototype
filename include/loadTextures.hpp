#ifndef LOADTEXTURES_H
#define LOADTEXTURES_H

#include <SDL2/SDL.h>

namespace texture {
	extern SDL_Texture
		*tileset,
		*units,
		*font,
		*ui,
		*fixed,
		*turret;
};

void loadTextures();
void freeTextures();

#endif
