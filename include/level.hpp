#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "map.hpp"
#include "unit.hpp"

class Unit;

class Level {
	std::vector<Unit> units;
public:
	Map map;
	Unit *unitAt(int x, int y);
	void load(const char *filename);
	void save(const char *filename);
	void update(int diff);
	void drawUnits(int x, int y);
	void addUnit(int x, int y, int type, int team);

	friend class Unit;
};

#endif
