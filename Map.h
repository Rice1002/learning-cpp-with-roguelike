#pragma once
#include "Common.h"
#include "Entity.h"

struct Room {
	int x, y, w, h;
};

class Map {
public:
	std::vector<std::vector<Tile>> tiles;
	std::vector<Room> rooms;

	Map();
	void generate(std::vector<std::unique_ptr<Entity>>& entities);

private:
	bool overlaps(const Room& a, const Room& b);
	void carveHLine(int x1, int x2, int y);
	void carveVLine(int y1, int y2, int x);
};