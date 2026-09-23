#pragma once
#include "Common.h"
class Map;
class Entity {
private:
	int x = 0, y = 0;
	char symbol = '?';
	std::string color;
	bool alive = true;
	int id;           // 唯一标识，用于 tie-breaker
	int energy;       // 当前能量，攒够 ENERGY_THRESHOLD 就能动
	int speed;        // 每 tick 累积的能量，基准 100
public:
	// 构造函数：所有数据必须在创建时初始化
	Entity(int startX, int startY, char sym, std::string col, int entityId)
		: x(startX), y(startY), symbol(sym), color(col), alive(true),id(entityId), energy(0), speed(PLAYER_BASE_SPEED) {} 


	// 虚析构函数
	virtual ~Entity() = default;

	// 纯虚函数，子类必须实现
	virtual void update(const Entity& player, const Map& map) = 0;

	// ===== Getter 接口（只读，带 const） =====
	int getX() const { return x; }
	int getY() const { return y; }
	char getSymbol() const { return symbol; }
	std::string getColor() const { return color; }
	bool isAlive() const { return alive; }
	int getId() const { return id; }
	int getEnergy() const { return energy; }
	int getSpeed() const { return speed; }

	// 增加能量（每 tick 调度器调用）
	void addEnergy(int amount) { energy += amount; }

	// 消耗能量（行动后调用）
	void consumeEnergy(int amount) { energy -= amount; }

	// 修改速度（给词缀/状态效果预留）
	void setSpeed(int newSpeed) { speed = newSpeed; }

	// ===== Setter 接口（修改数据） =====
	void setPosition(int newX, int newY) { x = newX; y = newY; }
	void kill() { alive = false; }
};

class Player : public Entity {
public:
	Player(int startX, int startY, int entityId); // 构造函数声明
	void update(const Entity& player, const Map& map) override;
};

class Monster : public Entity {
public:
	Monster(int startX, int startY, int entityId);
	void update(const Entity& player, const Map& map) override;
};