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

Map Level::generatePathmap(int x1, int y1, int x2, int y2) {
	Map m;
	m.init(map.w, map.h, -1);

	for(int i = 0; i < m.w*m.h; i++)
		m.arr[i] = map.tileBlocks(map.arr[i])*-1;
	for(int i = 0; i < units.size(); i++)
		m.setTile(units.at(i).x, units.at(i).y, -1);

	m.setTile(x1, y1, 1);

	for(int i = 1; m.getTile(x2, y2) == 0; i++)
		for(int j = 0; j < m.w*m.h; j++) {
			if(m.arr[j] != i)
				continue;

			int x = j%m.w, y = j/m.w;
			for(int xm = -1; xm <= 1; xm++)
				for(int ym = -1; ym <= 1; ym++) {
					if(m.getTile(x+xm, y+ym) != 0)
						continue;
					/*if(xm && ym)
						continue;*/
					m.setTile(x+xm, y+ym, i+1);
				}
		}

	return m;
}
