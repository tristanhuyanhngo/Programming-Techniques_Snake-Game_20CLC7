#include "Header.h"

// Global Variables
//-- Snake
Coordinates appearance{ 20, 20 };
const string SNAKE_SHAPE = "@19127095191273921912745919127476";
int SNAKE_LENGTH = 9;
int SNAKE_INDEX = 9;

//-- Map
Map information_board{ 2, 18, 110, 150, {} };
Map gameplay_guide{ 20, 40, 110, 150, {} };
Map original_map{ 2, 40, 10 , 100, {} };

// ___________________________________ Background ___________________________________
void SetBackGround() {
	SetConsoleCP(437);
	SetConsoleOutputCP(437);
	system("Color F0");
}

void setTextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
//-----------------------------------------------------------------------
void SetColor(WORD color) {
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

// ___________________________________ Position ___________________________________
int WhereX() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.X;
	return -1;
}
//-----------------------------------------------------------------------
int WhereY() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.Y;
	return -1;
}
//-----------------------------------------------------------------------
void GotoXY(int x, int y) {
	static HANDLE hConsoleOutput = NULL;
	if (!hConsoleOutput) {
		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	COORD Cursor_an_Pos = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}
//-----------------------------------------------------------------------
void HiddenCursor(bool CursorVisibility) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}
//-----------------------------------------------------------------------
int InputKey(int CheckPosition) {
	if (_kbhit()) {
		int key = _getch();
		if (key == 119 || key == 115 || key == 100 || key == 97) {
			if (key == 119 && CheckPosition != 0) {
				CheckPosition = 1;
			}
			else if (key == 115 && CheckPosition != 1) {
				CheckPosition = 0;
			}
			else if (key == 100 && CheckPosition != 3) {
				CheckPosition = 2;
			}
			else if (key == 97 && CheckPosition != 2) {
				CheckPosition = 3;
			}
		}
		return CheckPosition;
	}

	return KEY_NONE;
}
//-----------------------------------------------------------------------
void ChangeColor(int desiredColor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), desiredColor);
}

// ___________________________________________________ Map ___________________________________________________
void DrawMap(Map map) {
	int x = map.left;
	int y = map.top;

	// Top
	while (x <= map.right) {
		GotoXY(x, y);
		cout << (char)219;
		x++;
	}

	// Bottom
	x = map.left;
	y = map.bottom;
	while (x <= map.right) {
		GotoXY(x, y);
		cout << (char)219;
		x++;
	}

	// Right
	x = map.right;
	y = map.top;
	while (y <= map.bottom) {
		GotoXY(x, y);
		cout << (char)219;
		y++;
	}

	// Left
	x = map.left;
	y = map.top;
	while (y <= map.bottom) {
		GotoXY(x, y);
		cout << (char)219;
		y++;
	}
}

void DrawInformationBoard(Map map, int speed, int level, int score) {
	int x = map.right + 10;
	int y = map.top + 2;

	GotoXY(x, y);
	cout << "Level: " << level;
	y += 2;
	GotoXY(x, y);
	cout << "Speed: " << speed;
	y += 2;
	GotoXY(x, y);
	cout << "Score: " << score;
}

// ___________________________________________________ Snake ___________________________________________________
void InitializeSnake(vector<Chain>& Snake) {
	Chain object;
	int x = appearance.x, y = appearance.y;

	for (int i = 0; i < SNAKE_LENGTH; i++) {
		object.block = SNAKE_SHAPE[i];
		object.position.x = x;
		object.position.y = y;
		x--;

		Snake.push_back(object);
	}
}
//-----------------------------------------------------------------------
void DrawSnake(vector<Chain> Snake) {
	for (unsigned int i = 0; i < Snake.size(); i++) {
		GotoXY(Snake[i].position.x, Snake[i].position.y);
		SetColor(2);
		cout << Snake[i].block;
	}
}
//-----------------------------------------------------------------------
void HiddenMovement(vector<Chain> Snake) {
	for (unsigned int i = 0; i < Snake.size(); i++) {
		GotoXY(Snake[i].position.x, Snake[i].position.y);
		cout << " ";
	}
}
//-----------------------------------------------------------------------
void MakeSnakeMovement(vector<Chain>& Snake, int x, int y) {
	for (size_t i = Snake.size() - 1; i > 0; i--) {
		Snake[i].position = Snake[static_cast<std::vector<Chain, std::allocator<Chain>>::size_type>(i) - 1].position;
	}

	DrawSnake(Snake);
}
void GameRunning(vector<Chain>& Snake, int x, int y, Coordinates& food, Map map, int& count_food, Player& player, int level) {
	Coordinates last_position = Snake.back().position;
	MakeSnakeMovement(Snake, x, y);

	Snake[0].position = { x, y };
	if (CheckSnakeEatFood(Snake, food)) {
		if (SNAKE_INDEX == SNAKE_SHAPE.size() - 1) {
			SNAKE_INDEX = 2;
		}

		Chain temp;
		temp.block = SNAKE_SHAPE[SNAKE_INDEX];
		temp.position.x = last_position.x;
		temp.position.y = last_position.y;
		SNAKE_INDEX++;

		Snake.push_back(temp);
		count_food++;
		player.score += level;
		// Create Food
		food = InitializeFood(map, Snake);
	}
}
//-----------------------------------------------------------------------
bool CheckWallCollision(int x, int y, Map map) {
	if (x >= map.left && x <= map.right) {
		if (y == map.top || y == map.bottom) {
			return true;
		}
	}

	if (y >= map.top && y <= map.bottom) {
		if (x == map.left || x == map.right) {
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool CheckSnakeCollision(vector<Chain> Snake, int x, int y) {
	for (unsigned int i = 1; i < Snake.size(); i++) {
		if ((x == Snake[i].position.x) && (y == Snake[i].position.y)) {
			return true;
		}
	}

	return false;
}

bool CheckSnakeEatFood(vector<Chain> Snake, Coordinates food) {
	int x = Snake[0].position.x;
	int y = Snake[0].position.y;

	if (x == food.x && y == food.y) {
		return true;
	}

	return false;
}

bool ValidateFood(Coordinates food, Map map, vector<Chain> Snake) {
	for (unsigned int i = 0; i < Snake.size(); i++) {
		if (food.x == Snake[i].position.x && food.y == Snake[i].position.y) {
			return false;
		}
	}

	for (unsigned int i = 0; i < map.obstacles.size(); i++) {
		if (food.x == map.obstacles[i].x && food.y == map.obstacles[i].y) {
			return false;
		}
	}

	return true;
}

Coordinates InitializeFood(Map map, vector<Chain> Snake) {
	Coordinates food = { 0, 0 };

	do {
		food.x = rand() % (map.right - map.left - 2) + map.left + 1;
		food.y = rand() % (map.bottom - map.top - 2) + map.top + 1;
	} while (!ValidateFood(food, map, Snake));

	GotoXY(food.x, food.y);
	SetColor(4);
	cout << (char)235;

	return food;
}
// ___________________________________ Save & Load Game ___________________________________
void ClearData()
{
	fstream f;
	int choice = 0;
	f.open(FILENAME, ios::out);
	f.close();
	GotoXY(49, 18);
	SetColor(5);
	cout << "!! Data has been clean !!";
	SetColor(7);
	GotoXY(47, 19);
	system("pause");
	SetColor(0);
	//EnterGame(choice);
}
void Menu::SaveGame(string name, POINT& p)
{
	fstream f;
	// Ghi: name - toa do Passer Y - speed - so nguoi choi pass - toa do cac passer Y da qua - ve xe...
	f.open(FILENAME, ios::out | ios::app);
	f << endl;
	f.close();
}

void LoadData_Name(int& idx)
{
	fstream f;
	string line;
	f.open(FILENAME, ios::in);
	idx = 0;
	getline(f, line);
	while (!f.eof() && line != "")
	{
		int i = 0;
		GotoXY(54, 10 + idx);
		cout << ++idx << ". ";
		while (line[i] != ';')
		{
			if (line[i] != ';')
				cout << line[i++];
		}
		cout << "\n";
		getline(f, line);
	}
	f.close();
}
void Menu::SaveLoad()
{
	int idx, choice;
	do
	{
		system("cls");
		SetColor(4);		// Red color
		GotoXY(44, 5);
		printf("CSC10002 - PROGRAMMING TECHNIQUES");
		GotoXY(51, 6);
		cout << "GAME: SNAKE";
		GotoXY(54, 8);
		cout << "-- LOAD GAME --";
		SetColor(3);		// Cyan color
		LoadData_Name(idx);		// load game
		if (idx != 0)
		{
			SetColor(4);
			GotoXY(54, 12 + idx);
			cout << "Choose: ";
			cin >> choice;
		}
		else
		{
			GotoXY(50, 12);
			cout << "No data has been saved!! ^^";
			SetColor(7);
			GotoXY(46, 13);
			system("pause");
			SetColor(0);
			//EnterGame(choice);
			break;
		}
	} while (choice > idx || choice < 1);

	//PlayGame(idx);
}

bool IsVictory(int level, int count_food) {
	if (level == 4 && count_food == 4) {
		return true;
	}

	return false;
}

void WriteFile(fstream& fs, Player player) {
	fs.open("Score.txt", ios::out | ios::app);
	player.name = "";
	if (fs) {
		cout << "Enter your name: ";
		getline(cin, player.name);
		fs << player.name << " " << player.score << "\n";
	}
	else {
		cout << " Could not open file to write" << endl;
		fs.close();
		exit(EXIT_FAILURE);
	}
	fs.close();
}

// ___________________________________________________ Start Game ___________________________________________________
void Menu::StartGame(vector<Chain> Snake) {
	fstream fs;
	int X0 = 0;
	int Y0 = 0;

	// Map 1
	bool Gameover = false;
	int count_food = 0;

	Player player;
	player.score = 0;
	int speed = 10, level = 1;

	DrawMap(original_map);
	DrawInformationBoard(original_map, speed, level, player.score);
	InitializeSnake(Snake);
	Coordinates food = InitializeFood(original_map, Snake);
	DrawSnake(Snake);

	// Movement
	int x = appearance.x, y = appearance.y;
	int CheckPosition = 2;
	while (Gameover == false) {
		HiddenMovement(Snake);
		DrawInformationBoard(original_map, speed, level, player.score);
		if (_kbhit()) {
			int key = _getch();

			while (key == 112) {
				X0 = original_map.right + 10;
				Y0 = original_map.top + 8;
				GotoXY(X0, Y0);
				SetColor(4);
				cout << "PAUSE GAME ";
				key = _getch();
			}

			X0 = original_map.right + 10;
			Y0 = original_map.top + 8;
			GotoXY(X0, Y0);
			cout << "           ";

			if (key == 119 || key == 115 || key == 100 || key == 97) {
				if (key == 119 && CheckPosition != 0) {
					CheckPosition = 1;
				}
				else if (key == 115 && CheckPosition != 1) {
					CheckPosition = 0;
				}
				else if (key == 100 && CheckPosition != 3) {
					CheckPosition = 2;
				}
				else if (key == 97 && CheckPosition != 2) {
					CheckPosition = 3;
				}
			}
		}
		if (CheckPosition == 0) {
			y++;
		}
		else if (CheckPosition == 1) {
			y--;
		}
		else if (CheckPosition == 2) {
			x++;
		}
		else if (CheckPosition == 3) {
			x--;
		}
		GameRunning(Snake, x, y, food, original_map, count_food, player, level);
		// Is Victory
		if (IsVictory(level, count_food)) {
			system("cls");
			setTextColor(13);

			for (int i = startX; i <= endX - 6; i++) {
				setTextColor(6);
				GotoXY(i + 5, 8);
				putchar(177);
				GotoXY(i + 5, 19);
				putchar(177);
			}
			for (int i = startY; i <= endY - 14; i++) {
				setTextColor(6);
				GotoXY(100, i + 7);
				putchar(177);
				GotoXY(15, i + 7);
				putchar(177);
			}
			setTextColor(6);
			GotoXY(endX / 2 + startX - 7, endY / 2 + startY - 2);
			cout << "YOU WIN! ^ ^ - ^ ^" << endl;
			cout << endl;
			setTextColor(5);
			GotoXY(endX / 2 + startX - 16, endY / 2 + startY);
			WriteFile(fs, player);
			SetColor(0);
			break;

		}

		// Level Up
		if (count_food == 4) {
			if (speed < 130) {
				speed += 40;
			}
			level++;
			count_food = 0;
		}

		// Check Collision
		bool CheckWall = CheckWallCollision(x, y, original_map);
		bool CheckSnake = CheckSnakeCollision(Snake, x, y);
		Gameover = CheckWall || CheckSnake;
		Sleep(150 - speed); // 150 100 50 25 10

		if (Gameover) {
			system("cls");
			setTextColor(13);

			for (int i = startX; i <= endX - 6; i++) {
				setTextColor(6);
				GotoXY(i + 5, 8);
				putchar(177);
				GotoXY(i + 5, 19);
				putchar(177);
			}
			for (int i = startY; i <= endY - 14; i++) {
				setTextColor(6);
				GotoXY(100, i + 7);
				putchar(177);
				GotoXY(15, i + 7);
				putchar(177);
			}
			setTextColor(6);
			GotoXY(endX / 2 + startX - 7, endY / 2 + startY - 2);
			cout << "YOU LOSE! TT-TT" << endl;
			cout << endl;
			setTextColor(5);
			GotoXY(endX / 2 + startX - 16, endY / 2 + startY);
			if (player.score != 0) {
				WriteFile(fs, player);
			}
			SetColor(0);
			break;
		}
	}

	char keybuffer = _getch();
}

void Menu::Rankings() {
	fstream fs;
	fs.open("score.txt", ios::in);
	if (fs) {
		string score;
		getline(fs, score);
		vector<string> BestScore;
		vector<string> haystack = Tokenizer::Split(score);
		vector<string>Name;
		//cout << haystack[haystack.size() - 1]<< endl;
		BestScore.push_back(haystack[haystack.size() - 1]);
		stringstream ss;

		for (unsigned int i = 0; i < haystack.size() - 1; i++) {

			ss << haystack[i] << " ";
		}
		Name.push_back(ss.str());
		while (getline(fs, score)) {
			haystack.clear();
			haystack = Tokenizer::Split(score);
			BestScore.push_back(haystack[haystack.size() - 1]);

			stringstream ff;
			for (unsigned int i = 0; i < haystack.size() - 1; i++) {
				ff << haystack[i] << " ";
			}
			Name.push_back(ff.str());
		}
		string Score[100];
		string name[100];
		for (unsigned int i = 0; i < BestScore.size(); i++) {
			Score[i] = BestScore[i];
			name[i] = Name[i];
		}
		for (unsigned int i = 0; i < BestScore.size(); i++) {
			for (unsigned int j = i + 1; j < BestScore.size(); j++) {
				if (stoi(Score[i]) < stoi(Score[j])) {
					swap(Score[i], Score[j]);
					swap(name[i], name[j]);
					//cout << "YES" << endl;
				}
			}
		}
		SetColor(7);
		cout << "\t\t\t\t    __  _______ ______   __   _____ __________  ____  ______" << endl;
		cout << "\t\t\t\t   / / / /  _ / ____/ / / /  / ___// ____/ __ \\/ __ \\/ ____/" << endl;
		cout << "\t\t\t\t  / /_/ // / / / __/ /_/ /  \\__  \\/ /   / / / / /_/ / ___/   " << endl;
		cout << "\t\t\t\t / __  // / / /_/ / __  /   ___/ / /__ / /_/ / _,_ / /___   " << endl;
		cout << "\t\t\t\t/_/ /_/___/ \\____/_/ /_/   /____/\____/ \\____/_/ |_/_____/" << endl;

		for (int i = startX; i <= endX - 2; i++) {
			SetColor(5);
			GotoXY(i + 6, 7);
			putchar(177);
			GotoXY(i + 6, 26);
			putchar(177);
		}
		for (int i = startY; i <= endY - 6; i++) {
			SetColor(5);
			GotoXY(105, i + 6);
			putchar(177);
			GotoXY(16, i + 6);
			putchar(177);
		}
		SetColor(1);
		GotoXY(endX / 2 + startX - 55, endY / 2 + startY - 2);
		if (BestScore.empty()) {
			cout << "No score was save !!!" << endl;
		}
		else if (BestScore.size() <= 10) {
			for (unsigned int i = 0; i < BestScore.size(); i++) {
				GotoXY(52, 10 + i);
				cout << "Top " << i + 1 << " ";
				cout << name[i] << Score[i] << endl;

				//cout << BestScore[i] << endl;
			}
		}
		else {
			int i = 0;
			for (unsigned int j = BestScore.size() - 1; j >= BestScore.size() - 10; j--) {

				cout << "\t\t\t\t\t\t\tTop " << ++i << " ";
				cout << name[j] << Score[j] << endl;

			}
			BestScore.clear();
			Name.clear();
		}
	}
	else {
		GotoXY(endX / 2 + startX - 15, endY / 2 + startY - 2);
		cout << "No one saved their score :D \n";
		fs.close();
		EXIT_FAILURE;
	}
	fs.close();
}

void Menu::HowToPlay() {

	SetColor(7);
	cout << "\n";
	cout << "\t\t\t\t        _____              _              _         " << endl;
	cout << "\t\t\t\t       |  |  |___ _ _ _   | |_ ___    ___| |___ _ _ " << endl;
	cout << "\t\t\t\t       |     | . | | | |  |  _| . |  | . | | .'| | |   " << endl;
	cout << "\t\t\t\t       |__|__|___|_____|  |_| |___|  |  _|_|__,|_  |   " << endl;
	cout << "\t\t\t\t                                     |_|       |___|" << endl;

	for (int i = startX; i <= endX - 2; i++) {
		SetColor(5);
		GotoXY(i + 6, 7);
		cout << (char)219;
		GotoXY(i + 6, 26);
		cout << (char)219;
	}
	for (int i = startY; i <= endY - 6; i++) {
		SetColor(5);
		GotoXY(105, i + 6);
		cout << (char)219;
		GotoXY(16, i + 6);
		cout << (char)219;
	}
	GotoXY(35, 12);
	SetColor(5);
	cout << "\t\tNhan (W) - Di chuyen len tren" << endl;
	GotoXY(35, 13);
	cout << "\t\tNhan (S) - Di chuyen xuong duoi" << endl;
	GotoXY(35, 14);
	cout << "\t\tNhan (A) - Di chuyen qua trai" << endl;
	GotoXY(35, 15);
	cout << "\t\tNhan (D) - Di chuyen qua phai" << endl;
	GotoXY(35, 18);
	SetColor(8);
	cout << "\t\tNhan (Z) - De Save Game" << endl;
	GotoXY(35, 19);
	cout << "\t\tNhan (P) - De Pause Game" << endl;
}

int move() {

	char c = _getch();

	if ((int)c == -32) c = _getch();

	switch ((int)c) {

	case 80:
		return 1; //cout << "Xuong";

	case 72:
		return 2; //cout << "Len";

	case 77:
		return 3; //cout << "Phai";       

	case 75:
		return 4; //cout << "Trai";

	case 27:
		return 8; //Nut ESC thoat

	case 13:
		return 5; //Nut Enter

	default:
		return 0; //cout << "Sai";

	}

}

Menu::Menu() {

	item = new string[100];

	_numberOfItem = 6;

	item[0] = "PLAY";
	item[1] = "SAVE GAME";
	item[2] = "LOAD GAME";
	item[3] = "RANKING";
	item[4] = "HOW TO PLAY";
	item[5] = "EXIT";

}

Menu::Menu(vector<string> temp) {


	item = new string[100];

	_numberOfItem = temp.size();


	for (unsigned int i = 0; i < temp.size(); i++) {
		item[i] = temp[i];
	}


}

void Menu::ShowMenu() {

	GotoXY(5, 1);
	SetColor(3);
	cout << "\t\t                     _______  __    _  _______  ___   _  _______" << endl;
	cout << "\t\t                    |       ||  |  | ||   _   ||   | | ||       |" << endl;
	cout << "\t\t                    |  _____||   |_| ||  |_|  ||   |_| ||    ___|" << endl;
	cout << "\t\t                    | |_____ |       ||       ||      _||   |___" << endl;
	cout << "\t\t                    | _____ || _     ||       ||     |_ |   ___ |" << endl;
	cout << "\t\t                    ______| || | |   ||   _   ||    _  ||   |___" << endl;
	cout << "\t\t                    |_______||_| |___||__| |__||___| |_||_______|" << endl;



	setTextColor(10);
	for (int i = 0; i < _numberOfItem; i++) {

		GotoXY(50, i + 10);

		cout << "    " << item[i];

		Sleep(100); //Tam dung 100ms

	}


}


void Menu::deleteMenu() {

	for (int i = 0; i < _numberOfItem; i++) {

		Sleep(100);

		GotoXY(50, i + 10);

		for (unsigned int j = 0; j < item[i].size() + 3; j++) {

			cout << " ";

		}

	}
}

int Run(Menu* menu) {
	int x;

	int line = 10;

	bool thoat = false;
	system("cls");
	menu->ShowMenu();
	GotoXY(49, line);
	cout << (char)1;

	while (!thoat) {


		if (_kbhit()) {

			x = move();

			GotoXY(49, line);
			cout << " ";

			switch (x) {

			case 1:

			case 3:
			{

				line++;

				if (line >= menu->numberOfItem() + 10) line = 10;
				break;

			}

			case 2:

			case 4:
			{

				line--;

				if (line < 10) line = menu->numberOfItem() - 1 + 10;
				break;

			}

			case 5:
			{

				GotoXY(0, 0);
				cout << "                                           ";

				GotoXY(0, 0);
				cout << menu->getItem()[line];//"Ban chon " << 


				if (line == 10 && menu->getItem()[0] == "PLAY") {
					system("cls");
					vector<Chain> Snake;
					menu->StartGame(Snake);
				}
				else if (line == 11 && menu->getItem()[1] == "SAVE GAME") {

					system("cls");
					string name;
					POINT p;
					menu->SaveGame(name, p);
					GotoXY(45, 28);
					//system("pause");

				}
				else if (line == 12 && menu->getItem()[2] == "LOAD GAME") {
					system("cls");
					menu->SaveLoad();
					GotoXY(45, 28);
				}
				else if (line == 13 && menu->getItem()[3] == "RANKING") {
					system("cls");
					menu->Rankings();
					GotoXY(45, 28);
					system("pause");

				}
				else if (line == 14 && menu->getItem()[4] == "HOW TO PLAY") {
					system("cls");
					menu->HowToPlay();
					GotoXY(45, 28);
					system("pause");
				}
				else if (line == 15 && menu->getItem()[5] == "EXIT") {
					return 1;

				}

			}

			case 8:

				thoat = true;

			}
			GotoXY(49, line);
			cout << (char)1;

		}

	}
	menu->deleteMenu();
	return 0;
}