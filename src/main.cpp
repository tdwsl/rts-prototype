#include <SDL2/SDL.h>
#include "initSDL.hpp"
#include "loadTextures.hpp"
#include "game.hpp"

int main() {
	initSDL();
	loadTextures();

	Game game("lvl/0");
	game.run();

	freeTextures();
	endSDL();
	return 0;
}
