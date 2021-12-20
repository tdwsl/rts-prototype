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
}

void Team::update() {
	power = 0;
	for(int i = 0; i < level->units.size(); i++) {
		Unit &u = level->units.at(i);
		if(u.team != team)
			continue;
		if(u.power == 0)
			continue;
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
	}
}
