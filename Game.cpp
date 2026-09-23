#include "Game.h"
#include "Terminal.h"

Game::Game() {
	enableVT();
	hideCursor();
	map.generate(entities);
}

void Game::render() {
	clearScreen();
	std::string frameBuffer;

	for (int y = 0; y < MAP_H; ++y) {
		for (int x = 0; x < MAP_W; ++x) {
			bool entityDrawn = false;

			// 先画实体（这样实体可以覆盖在地板或墙上）
			for (const auto& e : entities) {
				if (e->getX() == x && e->getY() == y && e->isAlive()) {
					frameBuffer += e->getColor() + e->getSymbol() + RESET;
					entityDrawn = true;
					break;
				}
			}
			if (entityDrawn) continue;

			// 画地图
			if (map.tiles[y][x] == Tile::Wall)
				frameBuffer += WHITE + "#" + RESET;
			else
				frameBuffer += DARK_GRAY + "." + RESET;
		}
		frameBuffer += '\n';
	}
	frameBuffer += "wasd移动,q退出";
	std::cout << frameBuffer << std::flush;
}

void Game::handleInput() {
	int ch = _getch();
	if (entities.empty()) return;

	Player* player = dynamic_cast<Player*>(entities[0].get());
	if (!player) return;

	int newX = player->getX();
	int newY = player->getY();

	switch (ch) {
	case 'w': case 'W': newY--; break;
	case 's': case 'S': newY++; break;
	case 'a': case 'A': newX--; break;
	case 'd': case 'D': newX++; break;
	case 'q': case 'Q': running = false; break;
	}

	if (map.tiles[newY][newX] != Tile::Wall) {
		player->setPosition(newX,newY);
	}
}
void Game::processTurn() {
	// ========== 1. 所有存活实体累积能量 ==========
	for (auto& e : entities) {
		if (e->isAlive()) {
			e->addEnergy(e->getSpeed());
		}
	}

	// ========== 2. 收集能量达到阈值的实体 ==========
	// 用裸指针 vector 收集，避免复制 unique_ptr（所有权不能乱动）
	std::vector<Entity*> ready;
	for (auto& e : entities) {
		if (e->isAlive() && e->getEnergy() >= ENERGY_THRESHOLD) {
			ready.push_back(e.get());
		}
	}

	// ========== 3. 确定性排序：energy 降序，id 升序 ==========
	// 这是 GDD §5.1 反复强调的 tie-breaker，不写会破坏"同种子同结果"
	std::stable_sort(ready.begin(), ready.end(),
		[](Entity* a, Entity* b) {
			if (a->getEnergy() != b->getEnergy()) {
				return a->getEnergy() > b->getEnergy(); // 能量高的先动
			}
			return a->getId() < b->getId();             // 能量相同：id 升序
		});

	// ========== 4. 依次执行行动 ==========
	for (Entity* e : ready) {
		// 关键：这只实体可能在别人回合里死了，必须再查一次
		if (!e->isAlive()) continue;

		// 玩家（id == 0）特殊处理：阻塞等待输入
		if (e->getId() == 0) {
			handleInput();
		}
		else {
			e->update(*entities[0], map);
		}

		// 行动后消耗能量（先简化：固定消耗 100，日后可通过 cost 参数支持多回合行动）
		e->consumeEnergy(ENERGY_THRESHOLD);
	}
}
void Game::run() {
	while (running) {
		render();
		processTurn();  // 推进一个 tick
	}
	restoreCursor();

}