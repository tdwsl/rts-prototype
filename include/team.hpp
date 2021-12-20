#ifndef TEAM_H
#define TEAM_H

#include "level.hpp"

class Team {
	int wood, metal, stone, power, team;
	Level *level;
	bool factory, barracks, mine, farm;
public:
	Team(Level *level, int number, int wood, int metal, int stone);
	void update();

	friend class Sidebar;
};

#endif
