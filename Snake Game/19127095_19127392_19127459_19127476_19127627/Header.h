#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <sstream>
using namespace std;

#define MAX 100
#define KEY_NONE -1
#define startX 10
#define endX 100
#define startY 1
#define endY 26
#define FILENAME "Data.txt"

//----------
struct Coordinates {
	int x{};
	int y{};
};

struct Chain {
	char block{};
	Coordinates position{};
};

struct Map {
	int top{};
	int bottom{};
	int left{};
	int right{};
	vector <Coordinates> obstacles{};
};

struct Player {
	string name{};
	int score{};
};

// Function Prototype
// ___________________________________ Background ___________________________________
void SetBackGround();
void SetColor(WORD color);
void setTextColor(int color);
// ___________________________________ Position ___________________________________
int WhereX();
int WhereY();
void GotoXY(int x, int y);
void HiddenCursor(bool CursorVisibility);
int InputKey(int CheckPosition);
// ___________________________________ Map & Menu ___________________________________
void DrawMap(Map map);
void DrawInformationBoard(Map map, int speed, int level, int score);
// ___________________________________ Snake ___________________________________
void InitializeSnake(vector<Chain>& Snake);
void DrawSnake(vector<Chain> Snake);
void HiddenMovement(vector<Chain> Snake);
void MakeSnakeMovement(vector<Chain>& Snake, int x, int y);
void GameRunning(vector<Chain>& Snake, int x, int y, Coordinates& food, Map map, int& count_food, Player& player, int level);
bool CheckWallCollision(int x, int y, Map map);
bool CheckSnakeCollision(vector<Chain> Snake, int x, int y);
bool CheckSnakeEatFood(vector<Chain> Snake, Coordinates food);
// ___________________________________ Food ___________________________________
bool ValidateFood(Coordinates food, Map map, vector<Chain> Snake);
Coordinates InitializeFood(Map map, vector<Chain> Snake);
// ___________________________________ Save & Load Game ___________________________________
// 
// ___________________________________ Score ___________________________________
bool IsVictory(int level, int count_food);
void WriteFile(fstream& fs, Player player);
class Tokenizer {
public:
	static vector<string>Split(string haystack, string seperator = " ") {
		vector<string>Result;
		int start = 0;

		while (true) {
			size_t findpos = haystack.find(seperator, start);
			if (findpos != string::npos) {
				string token = haystack.substr(start, findpos - start);
				start = findpos + seperator.length();
				Result.push_back(token);
			}
			else {
				string token = haystack.substr(start, haystack.length() - start);
				Result.push_back(token);
				break;
			}

		}
		return Result;
	}
};

// ___________________________________ Menu ___________________________________

struct Menu {
	string* item;
	int _numberOfItem;

	Menu();
	Menu(vector<string>);
	void ShowMenu();
	void deleteMenu();
	~Menu() {}
	int numberOfItem() { return _numberOfItem; }
	string* getItem() { return item; }
	void StartGame(vector<Chain> Snake);
	void SaveLoad();
	void SaveGame(string name, POINT& p);
	void HowToPlay();
	void Rankings();
};

int move();// di chuyen menu
//void clrscr();

// ___________________________________ Start Game ___________________________________
int Run(Menu* a);



