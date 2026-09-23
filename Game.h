#pragma once
#include "Common.h"
#include "Map.h"
#include "Entity.h"

class Game {
public:
	Game();
	void run();

private:
	Map map;
	std::vector<std::unique_ptr<Entity>> entities;
	bool running = true;

	void render();
	void handleInput();
	void processTurn();
};