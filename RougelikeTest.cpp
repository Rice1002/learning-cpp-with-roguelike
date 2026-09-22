#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<vector>
#include<string>
const std::string RESET = "\033[0m";
const std::string YELLOW = "\033[1;33m";
const std::string WHITE = "\033[37m";
const std::string DARK_GRAY = "\033[90m";
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
struct Player {
	int x = 2; 
	int y = 2; 
};
std::vector<std::string> map = {
	"########################################",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"#......................................#",
	"########################################"
};
void render(const Player& player)
{
	clearScreen();
	for (int y = 0;y < map.size();++y)
	{
		for (int x = 0;x < map[y].size();++x)
		{
			if (player.x == x && player.y == y)
			{
				std::cout << YELLOW << '@' << RESET;
			}
			else
			{
				if (map[y][x] == '#')
				{
					std::cout << WHITE << '#' << RESET;
				}
				else
				{
					std::cout << DARK_GRAY <<'.' << RESET;
				}
			}
		}
		std::cout << '\n';
	}
	std::cout.flush();
}
int main()
{
	std::ios::sync_with_stdio(false);
	enableVT();
	hideCursor();
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
		if (map[newY][newX] != '#') {
			player.x = newX; 
			player.y = newY;
		}
	}
	restoreCursor();
	return 0;
}