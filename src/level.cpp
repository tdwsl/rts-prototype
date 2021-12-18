#include <SDL2/SDL.h>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "initSDL.hpp"
#include "unit.hpp"
#include "map.hpp"
#include "level.hpp"

void Level::addUnit(int x, int y, int type, int team) {
	Unit u(this, x, y, type, team);
	units.push_back(u);
}

Unit *Level::unitAt(int x, int y) {
	for(int i = 0; i < units.size(); i++)
		if(units.at(i).x == x && units.at(i).y == y)
			return &units.at(i);
	return 0;
}

void Level::update(int diff) {
	for(int i = 0; i < units.size(); i++) {
		units.at(i).update(diff);
		if(units.at(i).hp < 0) {
			units.erase(units.begin()+i);
			i--;
		}
	}
}

void Level::drawUnits(int x, int y) {
	for(int i = 0; i < units.size(); i++)
		units.at(i).draw(x, y);
}

void Level::load(const char *filename) {
	units.clear();

	char *mapfn = (char*)malloc(strlen(filename)+9);
	char *unitfn = (char*)malloc(strlen(filename)+11);
	sprintf(mapfn, "%s/map.txt", filename);
	sprintf(unitfn, "%s/units.txt", filename);

	map.load(mapfn);

	FILE *fp = fopen(unitfn, "r");
	assert(fp);
	int n;
	fscanf(fp, "%d", &n);
	for(int i = 0; i < n; i++) {
		int x, y, type, team;
		float hp;
		fscanf(fp, "%d%d%d%d%f", &x, &y, &type, &team, &hp);
		Unit u(this, x, y, type, team);
		if(hp >= 0)
			u.hp = hp;
		units.push_back(u);
	}
	fclose(fp);

	free(unitfn);
	free(mapfn);
}

void Level::save(const char *filename) {
	char *mapfn = (char*)malloc(strlen(filename)+9);
	char *unitfn = (char*)malloc(strlen(filename)+11);
	sprintf(mapfn, "%s/map.txt", filename);
	sprintf(unitfn, "%s/units.txt", filename);

	map.save(mapfn);

	FILE *fp = fopen(unitfn, "r");
	assert(fp);
	fprintf(fp, "%d\n", (int)units.size());
	for(int i = 0; i < units.size(); i++) {
		Unit &u = units.at(i);
		fprintf(fp, "%d %d %d %d %f\n",
			u.x, u.y, u.type, u.team, u.hp);
	}
	fclose(fp);

	free(unitfn);
	free(mapfn);
}
