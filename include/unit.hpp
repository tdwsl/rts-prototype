#ifndef UNIT_H
#define UNIT_H

#define PI 3.14159

#include "level.hpp"

enum {
	UNITTYPE_SOLDIER=0,
	UNITTYPE_DIGGER,
	UNITTYPE_HARVESTER,
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

	bool moving();
	SDL_Rect destRect(int x, int y);
public:
	Unit(Level *level, int x, int y, int type, int team);
	void move(int x, int y);
	void navTo(int tx, int ty);
	void update(int diff);
	void draw(int x, int y);
	void drawUI_bottom(int x, int y);
	void drawUI_top(int x, int y);
	void target(int x, int y);

	friend class Level;
};

#endif
