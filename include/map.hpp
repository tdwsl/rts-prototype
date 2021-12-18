#ifndef MAP_H
#define MAP_H

class Map {
	int *arr = 0;
	void print();
public:
	int w, h;

	~Map();
	void init(int w, int h, int t);
	void load(const char *filename);
	void save(const char *filename);
	void draw(int x, int y);
	int getTile(int x, int y);
	void setTile(int x, int y, int);
	Map generatePathmap(int x1, int y1, int x2, int y2);
	bool blocks(int x, int y);
};

#endif
