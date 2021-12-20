#ifndef TEAM_H
#define TEAM_H

#include "level.hpp"

class Team {
	int wood, metal, stone, power, team;
	Level *level;
public:
	Team(Level *level, int number, int wood, int metal, int stone);
	void update();
};

#endif
