#include <SDL2/SDL.h>
#include <cstdlib>
#include <cstdio>
#include "initSDL.hpp"
#include "loadTextures.hpp"
#include "level.hpp"
#include "unit.hpp"

struct stat {
	float hp, range, speed, damage, regen;
};

const stat stats[] = {
	{10, 15, 0.002, 2.5, 0},
	{20, 10, 0.001, 7, 0.1},
};

Unit::Unit(Level *level, int x, int y, int type, int team) {
	Unit::team = team;
	Unit::type = type;
	Unit::level = level;
	Unit::x = x;
	Unit::y = y;
	px = x;
	py = y;

	const stat &t = stats[type];
	hp = t.hp;
}

bool Unit::moving() {
	if(px != x || py != y)
		return true;
	return false;
}

void Unit::navTo(int tx, int ty) {
	if(moving())
		return;

	Map pmap = level->map.generatePathmap(x, y, tx, ty);

	int xm, ym;
	int i = pmap.getTile(tx, ty);
	int cx = tx, cy = ty;
	while(cx != x || cy != y) {
		xm = 0;
		ym = 0;
		bool found = false;

		for(int sxm = -1; sxm <= 1 && !found; sxm++)
			for(int sym = -1; sym <= 1 && !found; sym++) {
				if(!sxm || !sym)
					continue;
				if(pmap.getTile(cx+sxm, cy+sym) == i-1) {
					found = true;
					xm = sxm;
					ym = sym;
				}
			}

		for(int sxm = -1; sxm <= 1 && !found; sxm++)
			for(int sym = -1; sym <= 1 && !found; sym++)
				if(pmap.getTile(cx+sxm, cy+sym) == i-1) {
					found = true;
					xm = sxm;
					ym = sym;
				}

		i--;
		cx += xm;
		cy += ym;
		if(i <= 1)
			break;
	}

	move(-xm, -ym);
}

void Unit::move(int x, int y) {
	if(moving())
		return;
	if(!x && !y)
		return;
	
	int dx = Unit::x+x, dy = Unit::y+y;
	if(level->map.blocks(dx, dy))
		return;
	if(level->unitAt(dx, dy))
		return;

	int sd = 0;
	for(int sx = -1; sx <= 1; sx++)
		for(int sy = -1; sy <= 1; sy++) {
			if(sx == x*-1 && sy == y*-1)
				d = (sd+1)%8;
			sd++;
		}

	Unit::x = dx;
	Unit::y = dy;
	step = 0;
}

void Unit::update(int diff) {
	const stat &t = stats[type];

	if(hp < t.hp) {
		hp += t.regen*diff;
		if(hp > t.hp)
			hp = t.hp;
	}

	if(moving()) {
		step += t.speed*diff;
		if(step >= 1.0) {
			step = 0;
			px = x;
			py = y;
			pd = d;
		}
	}
}

void Unit::draw(int x, int y) {
	int t = 0;
	if((int)(step*4)%2)
		t = 1+(int)(step*8)%2;
	if(cooldown)
		t = 3;
	t += type*4;

	float ca = (PI/4)*d;
	float pa = (PI/4)*pd;
	float am = step*1.5;
	if(am > 1)
		am = 1;
	float a = pa+(ca-pa)*am;
	a = (a/PI)*180;

	SDL_Rect src = (SDL_Rect){(t%UNITS_W)*32, (t/UNITS_W)*32, 32, 32};
	int xd = Unit::x-px, yd = Unit::y-py;
	SDL_Rect dst = (SDL_Rect) {
		px*24 + xd*(int)(24*step) + x,
		py*24 + yd*(int)(24*step) + y,
		24, 24
	};
	SDL_Point p = (SDL_Point){12, 12};

	SDL_RenderCopyEx(renderer, texture::units, &src, &dst,
			a, &p, SDL_FLIP_NONE);
}
