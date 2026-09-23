#include "Entity.h"
#include"Map.h"

// 冒号后面是“初始化列表”，把参数直接传给基类 Entity 的构造函数
Player::Player(int startX, int startY,int entityId)
	: Entity(startX, startY, '@', YELLOW,entityId) {
} // 玩家：@ 黄色

void Player::update(const Entity& player, const Map& map) {
	// 玩家由键盘控制，暂时不写
	(void)player;
	(void)map;
}

Monster::Monster(int startX, int startY, int entityId)
	: Entity(startX, startY, 'M', RED, entityId) {
}

void Monster::update(const Entity& player, const Map& map) {
	// AI 逻辑下一步写，这里先留空
	(void)player;
	(void)map;
}