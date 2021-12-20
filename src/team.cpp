#include "level.hpp"
#include "unit.hpp"
#include "team.hpp"

Team::Team(Level *level, int team, int wood, int metal, int stone) {
	Team::level = level;
	Team::wood = wood;
	Team::metal = metal;
	Team::stone = stone;
	Team::team = team;
	power = 0;
	factory = false;
	barracks = false;
	mine = false;
	farm = false;
}

void Team::update() {
	power = 0;
	factory = false;
	barracks = false;
	mine = false;
	farm = false;

	for(int i = 0; i < level->units.size(); i++) {
		Unit &u = level->units.at(i);
		if(u.team != team)
			continue;

		if(u.type == UNITTYPE_BARRACKS)
			barracks = true;
		if(u.type == UNITTYPE_FACTORY)
			factory = true;
		if(u.type == UNITTYPE_MINE)
			mine = true;
		if(u.type == UNITTYPE_FARM)
			farm = true;

		power += u.power;
		u.disabled = false;
	}

	for(int i = level->units.size(); power < 0 && i < 0; i--) {
		Unit &u = level->units.at(i);
		if(u.team != team)
			continue;

		if(u.power >= 0)
			continue;
		power -= u.power;
		u.disabled = true;

		if(u.type == UNITTYPE_BARRACKS)
			barracks = false;
		if(u.type == UNITTYPE_FACTORY)
			factory = false;
		if(u.type == UNITTYPE_MINE)
			mine = false;
		if(u.type == UNITTYPE_FARM)
			farm = false;
	}
}
