#ifndef UNIT_H
#define UNIT_H

#define PI 3.14159

#include <SDL2/SDL.h>
#include "level.hpp"

enum {
	UNITTYPE_SOLDIER=0,
	UNITTYPE_DIGGER,
	UNITTYPE_HARVESTER,
	UNITTYPE_POWERPLANT,
	UNITTYPE_BARRACKS,
	UNITTYPE_FACTORY,
	UNITTYPE_FARM,
	UNITTYPE_MINE,
};

extern const char *unitNames[];

class Level;

class Unit {
	int team;
	int x, y, px, py;
	float step;
	int type;
	float hp, maxhp, regen;
	float speed;
	int cooldown;
	int d, pd;
	Level *level;
	bool fixed;
	int targetX, targetY, targetMode;
	int power;
	bool disabled, turret;
	int graphic;
	int w, h;
	int spawnX, spawnY;
	Unit *targetUnit;

	bool moving();
	SDL_Rect destRect(int x, int y);
	void drawFixed(int x, int y);
public:
	Unit(Level *level, int x, int y, int type, int team);
	void move(int x, int y);
	void navTo(int tx, int ty);
	void update(int diff);
	void draw(int x, int y);
	void drawUI_bottom(int x, int y);
	void drawUI_top(int x, int y);
	void target(int x, int y);
	void flatten();

	friend class Level;
	friend class Team;
};

#endif
