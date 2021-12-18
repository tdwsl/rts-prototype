#ifndef UNIT_H
#define UNIT_H

#define PI 3.14159

#include "level.hpp"

class Level;

class Unit {
	int team;
	int x, y, px, py;
	float step=0;
	int type;
	float hp;
	int cooldown=0;
	int d=0, pd=0;
	Level *level;

	bool moving();
public:
	Unit(Level *level, int x, int y, int type, int team);
	void move(int x, int y);
	void navTo(int tx, int ty);
	void update(int diff);
	void draw(int x, int y);

	friend class Level;
};

#endif
