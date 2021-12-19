#include <SDL2/SDL.h>
#include <cstdlib>
#include <cassert>
#include "loadTextures.hpp"
#include "initSDL.hpp"
#include "map.hpp"

#define TILESET_W 4

void Map::load(const char *filename) {
	FILE *fp = fopen(filename, "r");
	assert(fp);

	fscanf(fp, "%d%d", &w, &h);

	arr = (int*)realloc(arr, sizeof(int)*w*h);
	for(int i = 0; i < w*h; i++)
		fscanf(fp, "%d", &arr[i]);

	fclose(fp);
}

void Map::save(const char *filename) {
	FILE *fp = fopen(filename, "w");
	assert(fp);

	fprintf(fp, "%d %d\n", w, h);

	for(int i = 0; i < w*h; i++) {
		fprintf(fp, "%d", arr[i]);
		if(((i+1)%w))
			fprintf(fp, " ");
		else
			fprintf(fp, "\n");
	}

	fclose(fp);
}

void Map::draw(int x, int y) {
	for(int i = 0; i < w*h; i++) {
		SDL_Rect src = (SDL_Rect) {
			(arr[i]%TILESET_W)*24, (arr[i]/TILESET_W)*24,
			24, 24
		};
		SDL_Rect dst = (SDL_Rect) {
			(i%w)*24+x, (i/w)*24+y,
			24, 24,
		};

		SDL_RenderCopy(renderer, texture::tileset, &src, &dst);
	}
}

int Map::getTile(int x, int y) {
	if(x >= 0 && y >= 0 && x < w && y < h)
		return arr[y*w+x];
	return -1;
}

void Map::setTile(int x, int y, int t) {
	if(x >= 0 && y >= 0 && x < w && y < h)
		arr[y*w+x] = t;
}

void Map::init(int w, int h, int t) {
	Map::w = w;
	Map::h = h;
	arr = (int*)realloc(arr, sizeof(int)*w*h);
	if(t < 0)
		return;
	for(int i = 0; i < w*h; i++)
		arr[i] = t;
}

Map::~Map() {
	if(arr)
		free(arr);
}

bool Map::tileBlocks(int t) {
	switch(t) {
		case 0:
		case 1:
		case 2:
		case 3:
			return false;
		default:
			return true;
	}
}

bool Map::blocks(int x, int y) {
	return tileBlocks(getTile(x, y));
}

void Map::print() {
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++)
			printf("%2d", arr[y*w+x]);
		printf("\n");
	}
	printf("\n");
}
