#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<vector>
#include<string>
#include <random>   // 现代 C++ 随机数库
const std::string RESET = "\033[0m";
const std::string YELLOW = "\033[1;33m";
const std::string WHITE = "\033[37m";
const std::string DARK_GRAY = "\033[90m";
const int MAP_W = 80;
const int MAP_H = 25;

// 定义格子类型
enum class Tile {
	Wall,
	Floor
};
struct Room {
	int x, y;     // 房间左上角坐标
	int w, h;     // 房间宽度和高度
};
struct Player {
	int x = 2;
	int y = 2;
};

// 动态二维地图
std::vector<std::vector<Tile>> map;
void enableVT()
{
	HANDLE hOUT = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOUT, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOUT, dwMode);
}
void hideCursor()
{
	std::cout << "\033[?25l";
}
void restoreCursor()
{
	std::cout<<"\033[?25h";
}
void clearScreen()
{
	std::cout << "\033[H";
}

bool overlaps(const Room& a, const Room& b) {
	// 如果 a 的右边 <= b 的左边，或者 b 的右边 <= a 的左边，说明左右不重叠
	// 如果 a 的下边 <= b 的上边，或者 b 的下边 <= a 的上边，说明上下不重叠
	// 以上四种情况只要满足一种，就不重叠，否则就是重叠了
	bool noOverlap = (a.x + a.w <= b.x) || (b.x + b.w <= a.x) ||
		(a.y + a.h <= b.y) || (b.y + b.h <= a.y);
	return !noOverlap;
}
void initMap() {
	// 1. 先把整个地图填满墙
	map.assign(MAP_H, std::vector<Tile>(MAP_W, Tile::Wall));

	// 2. 初始化随机数引擎（用当前时间做种子，保证每次运行都不一样）
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> widthDist(5, 14);  // 房间宽 5~14
	std::uniform_int_distribution<> heightDist(4, 9);  // 房间高 4~9
	std::uniform_int_distribution<> xDist(1, MAP_W - 16); // x 坐标
	std::uniform_int_distribution<> yDist(1, MAP_H - 11); // y 坐标

	std::vector<Room> rooms; // 存所有已生成的房间

	// 3. 尝试生成 20 次房间（有些会因为重叠而失败）
	for (int i = 0; i < 20; ++i) {
		Room newRoom;
		newRoom.w = widthDist(gen);
		newRoom.h = heightDist(gen);
		newRoom.x = xDist(gen);
		newRoom.y = yDist(gen);

		// 4. 检查这个新房间是否和已有的房间重叠
		bool collides = false;
		for (const auto& r : rooms) {
			if (overlaps(newRoom, r)) {
				collides = true;
				break;
			}
		}

		// 5. 如果不重叠，就正式把这个房间挖出来
		if (!collides) {
			rooms.push_back(newRoom);
			// 遍历房间区域，把墙变成地板
			for (int y = newRoom.y; y < newRoom.y + newRoom.h; ++y) {
				for (int x = newRoom.x; x < newRoom.x + newRoom.w; ++x) {
					map[y][x] = Tile::Floor;
				}
			}
		}
	}
}
void render(const Player& player) {
	clearScreen();

	std::string frameBuffer; // 帧缓冲区，拼好一整帧后一次性输出

	for (int y = 0; y < MAP_H; ++y) {
		for (int x = 0; x < MAP_W; ++x) {
			if (player.x == x && player.y == y) {
				frameBuffer += YELLOW + "@" + RESET;
			}
			else if (map[y][x] == Tile::Wall) {
				frameBuffer += WHITE + "#" + RESET;
			}
			else {
				frameBuffer += DARK_GRAY + "." + RESET;
			}
		}
		frameBuffer += '\n'; // 每行结束换行
	}

	frameBuffer += "WASD 移动, Q 退出";

	// 一次性输出整帧画面，彻底解决卡顿
	std::cout << frameBuffer << std::flush;
}
int main()
{
	std::ios::sync_with_stdio(false);
	enableVT();
	hideCursor();
	initMap();
	Player player;
	bool running_flag = true;
	while (running_flag)
	{
		render(player);
		std::cout << "环境准备完毕！按任意键退出...\n";
		int ch=_getch();
		int newX = player.x;
		int newY = player.y;

		switch (ch) {
		case 'w': case 'W': newY--; break; // 向上
		case 's': case 'S': newY++; break; // 向下
		case 'a': case 'A': newX--; break; // 向左
		case 'd': case 'D': newX++; break; // 向右
		case 'q': case 'Q': running_flag = false; break; // 退出
		}
		// 检查新位置是不是墙（'#'）
		if (map[newY][newX] != Tile::Wall) {
			player.x = newX; 
			player.y = newY;
		}
	}
	restoreCursor();
	return 0;
}