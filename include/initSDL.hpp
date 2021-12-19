#ifndef INITSDL_H
#define INITSDL_H

#include <SDL2/SDL.h>

extern SDL_Renderer *renderer;
extern SDL_Window *win;

void initSDL();
void endSDL();
SDL_Texture *loadTexture(const char *filename);
Uint32 getMouseXY(int *x, int *y);
void updateDisplay();

#endif
