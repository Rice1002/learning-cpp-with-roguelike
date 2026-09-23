#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <algorithm>
#include <conio.h>
#define NOMINMAX
#include <windows.h>

// 颜色常量
const std::string RESET = "\033[0m";
const std::string YELLOW = "\033[1;33m";
const std::string WHITE = "\033[37m";
const std::string DARK_GRAY = "\033[90m";
const std::string RED = "\033[31m"; 
// 地图尺寸
const int MAP_W = 80;
const int MAP_H = 25;

// 回合能量系统（GDD §5.1）
// 玩家 speed = 100 是基准，定义了"1 回合"这个单位
// ENERGY_THRESHOLD 与 base speed 相等，保证基准实体每 tick 行动一次
const int PLAYER_BASE_SPEED = 100;
const int ENERGY_THRESHOLD = 100;

// 饥饿系统（GDD §5.7）—— 编译期开关，调试期可关
constexpr bool ENABLE_HUNGER = false;

// 最大深度（GDD §4）
const int MAX_DEPTH = 5;

// FOV 半径（GDD §5.2）—— 应恰好覆盖一个满房间
const int FOV_RADIUS = 8;

// 格子类型
enum class Tile { Wall, Floor };