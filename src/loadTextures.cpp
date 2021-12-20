#include <SDL2/SDL.h>
#include "loadTextures.hpp"
#include "initSDL.hpp"

#define loadtex(A) texture::A = loadTexture("img/"#A".bmp")
#define freetex(A) SDL_DestroyTexture(texture::A)
#define decltex(A) SDL_Texture *texture::A = NULL

decltex(tileset);
decltex(units);
decltex(font);
decltex(ui);
decltex(fixed);
decltex(turret);

void loadTextures() {
	loadtex(tileset);
	loadtex(units);
	loadtex(font);
	loadtex(ui);
	loadtex(fixed);
	loadtex(turret);
}

void freeTextures() {
	freetex(turret);
	freetex(fixed);
	freetex(tileset);
	freetex(units);
	freetex(font);
	freetex(ui);
}
