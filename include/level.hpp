#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "map.hpp"
#include "unit.hpp"
#include "team.hpp"

class Unit;

class Level {
	std::vector<Unit> units;
	std::vector<Team> teams;
	Map map;
	int playerTeam;
public:
	Unit *unitAt(int x, int y);
	void load(const char *filename);
	void save(const char *filename);
	void update(int diff);
	void drawUnits_bottom(int x, int y);
	void drawUnits_top(int x, int y);
	void addUnit(int x, int y, int type, int team);
	Map generatePathmap(int x1, int y1, int x2, int y2);

	friend class Unit;
	friend class Team;
	friend class Game;
};

#endif
