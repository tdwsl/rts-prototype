#include <SDL2/SDL.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "initSDL.hpp"
#include "loadTextures.hpp"
#include "level.hpp"
#include "unit.hpp"

#define UNITS_W 8

const char *unitNames[] = {
	"Soldier",
	"Digger",
	"Harvester",
};

Unit::Unit(Level *level, int x, int y, int type, int team) {
	cooldown = 0;
	step = 0;
	d = 0;
	pd = 0;
	targetMode = 0;

	Unit::team = team;
	Unit::type = type;
	Unit::level = level;
	Unit::x = x;
	Unit::y = y;
	px = x;
	py = y;

	fixed = false;
	regen = 0;
	power = 0;
	disabled = false;

	switch(type) {
		case UNITTYPE_SOLDIER:
			hp = 10;
			speed = 0.002;
			break;
		case UNITTYPE_DIGGER:
			hp = 15;
			speed = 0.0015;
			break;
		case UNITTYPE_HARVESTER:
			hp = 11;
			speed = 0.003;
			break;
	}

	maxhp = hp;
}

bool Unit::moving() {
	if(px != x || py != y)
		return true;
	return false;
}

void Unit::navTo(int tx, int ty) {
	if(moving())
		return;
	if(fixed)
		return;

	Map pmap = level->generatePathmap(x, y, tx, ty);

	int xm, ym;
	int i = pmap.getTile(tx, ty);
	int cx = tx, cy = ty;
	while(cx != x || cy != y) {
		xm = 0;
		ym = 0;
		bool found = false;

		for(int sxm = -1; sxm <= 1 && !found; sxm++)
			for(int sym = -1; sym <= 1 && !found; sym++) {
				if(sxm && sym)
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
	if(fixed)
		return;
	
	int dx = Unit::x+x, dy = Unit::y+y;
	if(level->map.blocks(dx, dy))
		return;
	if(level->unitAt(dx, dy))
		return;

	d = 0;
	const int dirs[] = {
		0, -1,
		1, -1,
		1, 0,
		1, 1,
		0, 1,
		-1, 1,
		-1, 0,
		-1, -1,
	};
	for(int i = 0; i < 8 && !d; i++)
		if(x == dirs[i*2] && y == dirs[i*2+1])
			d = i;

	Unit::x = dx;
	Unit::y = dy;
	step = 0;
}

void Unit::update(int diff) {
	if(hp < maxhp) {
		hp += regen*diff;
		if(hp > maxhp)
			hp = maxhp;
	}

	if(moving()) {
		step += speed*diff;
		if(step >= 1.0) {
			step = 0;
			px = x;
			py = y;
			pd = d;
		}
	}
	else if(targetMode == 1) {
		if(px == targetX && py == targetY)
			targetMode = 0;
		else
			navTo(targetX, targetY);
	}
}

SDL_Rect Unit::destRect(int x, int y) {
	int xd = Unit::x-px, yd = Unit::y-py;
	SDL_Rect dst = (SDL_Rect) {
		px*24 + xd*(int)(24*step) + x,
		py*24 + yd*(int)(24*step) + y,
		24, 24
	};
	return dst;
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

	float am = (abs(pd-d)>1) ? step*1.5 : step*2;
	if(am > 1)
		am = 1;

	float ad = (ca-pa);
	if(ad > PI)
		ad -= PI*2;
	if(ad < -PI)
		ad += PI*2;

	float a = pa + ad*am;
	a = (a/PI)*180;

	SDL_Rect src = (SDL_Rect){(t%UNITS_W)*32, (t/UNITS_W)*32, 32, 32};
	SDL_Rect dst = destRect(x, y);
	SDL_Point p = (SDL_Point){12, 12};

	SDL_RenderCopyEx(renderer, texture::units, &src, &dst,
			a, &p, SDL_FLIP_NONE);
}

void Unit::drawUI_top(int x, int y) {
	SDL_Rect src = (SDL_Rect){
		24*(int)((SDL_GetTicks()/200)%2), 240,
		24, 24
	};
	SDL_Rect dst = destRect(x, y);
	SDL_RenderCopy(renderer, texture::ui, &src, &dst);

	if(targetMode == 2) {
		src.x = 48;
		dst = (SDL_Rect){x+targetX*24, y+targetY*24, 24, 24};
		SDL_RenderCopy(renderer, texture::ui, &src, &dst);
	}
}

void Unit::drawUI_bottom(int x, int y) {
	if(!targetMode)
		return;

	int tx = targetX, ty = targetY;

	SDL_Rect dst1 = destRect(x, y);
	SDL_Rect dst2 = (SDL_Rect){x+tx*24, y+ty*24, 24, 24};

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x88);
	SDL_RenderDrawLine(renderer, dst1.x+12, dst1.y+12, dst2.x+12, dst2.y+12);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);

	if(targetMode == 1) {
		SDL_Rect src = (SDL_Rect){48, 240, 24, 24};
		SDL_RenderCopy(renderer, texture::ui, &src, &dst2);
	}
}

void Unit::target(int x, int y) {
	if(level->map.blocks(x, y))
		return;
	if(fixed)
		return;

	targetX = x;
	targetY = y;
	targetMode = 1;
}
