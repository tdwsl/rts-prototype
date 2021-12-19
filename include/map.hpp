#ifndef MAP_H
#define MAP_H

class Level;

class Map {
	int *arr = 0;
	void print();
	bool tileBlocks(int t);
public:
	int w, h;

	~Map();
	void init(int w, int h, int t);
	void load(const char *filename);
	void save(const char *filename);
	void draw(int x, int y);
	int getTile(int x, int y);
	void setTile(int x, int y, int);
	bool blocks(int x, int y);

	friend class Level;
};

#endif
