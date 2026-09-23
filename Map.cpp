#include "Map.h"

Map::Map() {
	tiles.assign(MAP_H, std::vector<Tile>(MAP_W, Tile::Wall));
}

bool Map::overlaps(const Room& a, const Room& b) {
	bool noOverlap = (a.x + a.w <= b.x) || (b.x + b.w <= a.x) ||
		(a.y + a.h <= b.y) || (b.y + b.h <= a.y);
	return !noOverlap;
}

void Map::carveHLine(int x1, int x2, int y) {
	for (int x = (std::min)(x1, x2); x <= (std::max)(x1, x2); ++x)
		tiles[y][x] = Tile::Floor;
}

void Map::carveVLine(int y1, int y2, int x) {
	for (int y = (std::min)(y1, y2); y <= (std::max)(y1, y2); ++y)
		tiles[y][x] = Tile::Floor;
}

void Map::generate(std::vector<std::unique_ptr<Entity>>& entities) {
	// 1. 重置地图
	tiles.assign(MAP_H, std::vector<Tile>(MAP_W, Tile::Wall));
	rooms.clear();

	// 2. 随机引擎
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> widthDist(5, 14);
	std::uniform_int_distribution<> heightDist(4, 9);
	std::uniform_int_distribution<> xDist(1, MAP_W - 16);
	std::uniform_int_distribution<> yDist(1, MAP_H - 11);

	// 3. 生成房间（这里保留你之前写的逻辑，循环 20 次，挖房间）
	for (int i = 0; i < 20; ++i) {
		Room newRoom;
		newRoom.w = widthDist(gen);
		newRoom.h = heightDist(gen);
		newRoom.x = xDist(gen);
		newRoom.y = yDist(gen);

		bool collides = false;
		for (const auto& r : rooms) {
			if (overlaps(newRoom, r)) { collides = true; break; }
		}

		if (!collides) {
			rooms.push_back(newRoom);
			for (int y = newRoom.y; y < newRoom.y + newRoom.h; ++y)
				for (int x = newRoom.x; x < newRoom.x + newRoom.w; ++x)
					tiles[y][x] = Tile::Floor;
		}
	}

	// 4. 挖走廊（保留你之前写的逻辑）
	for (size_t i = 1; i < rooms.size(); ++i) {
		int x1 = rooms[i - 1].x + rooms[i - 1].w / 2;
		int y1 = rooms[i - 1].y + rooms[i - 1].h / 2;
		int x2 = rooms[i].x + rooms[i].w / 2;
		int y2 = rooms[i].y + rooms[i].h / 2;

		std::uniform_int_distribution<> dirDist(0, 1);
		if (dirDist(gen) == 0) {
			carveHLine(x1, x2, y1);
			carveVLine(y1, y2, x2);
		}
		else {
			carveVLine(y1, y2, x1);
			carveHLine(x1, x2, y2);
		}
	}

	// 5. 生成玩家和怪物
		// 5. 生成玩家和怪物
	if (!rooms.empty()) {
		int entityId = 0; // 实体 id 计数器，从 0 开始

		// 玩家：固定 id = 0（Game.cpp 里通过 id 0 找玩家）
		auto player = std::make_unique<Player>(
			rooms[0].x + rooms[0].w / 2,
			rooms[0].y + rooms[0].h / 2,
			entityId++
		);
		entities.push_back(std::move(player));

		// 怪物：每个房间一只，id 从 1 开始递增
		for (size_t i = 1; i < rooms.size(); ++i) {
			auto monster = std::make_unique<Monster>(
				rooms[i].x + rooms[i].w / 2,
				rooms[i].y + rooms[i].h / 2,
				entityId++
			);
			entities.push_back(std::move(monster));
		}
	}
}