#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include<string.h>
#include <thread>
#define screen_x 44
#define screen_y 35
#define borderxstart 3
#define borderxend 16
#define borderystart 0
#define borderyend 29
#define KEY_UP    38
#define KEY_LEFT  37
#define KEY_RIGHT 39
#define KEY_DOWN  40

HANDLE wHnd;
HANDLE rHnd;
DWORD fdwMode;
COORD bufferSize = { screen_x,screen_y };
CHAR_INFO consoleBuffer[screen_x * screen_y];
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
COORD characterPos = { 0,0 };

long score = 0;
int BlockNum = rand() % 7;
int rotation = 0;
struct BlockTemp
{
	char px[17];
	int cb[17];
};
BlockTemp bt[32];
int setMode() {
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}

int setConsole(int x, int y) {
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

void gotoxy(int x, int y) {
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void setcursor(bool visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];
}

void fill_buffer_to_console() {
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}

void clear_buffer() {
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 7;
		}
	}
}

void TETRIS() {
	int color = 0;
	int nextword = 1;
	int row = 2;
	int something[6][6][6]{
		{//T
		{1,1,1,1,1,1},
		{1,1,1,1,1,1},
		{0,0,1,1,0,0},
		{0,0,1,1,0,0},
		{0,0,1,1,0,0},
		{0,0,1,1,0,0}
		},
		{//E
		{1,1,1,1,1,1},
		{1,1,0,0,0,0},
		{1,1,1,1,1,1},
		{1,1,1,1,1,1},
		{1,1,0,0,0,0},
		{1,1,1,1,1,1}
		},
		{//T
		{1,1,1,1,1,1},
		{1,1,1,1,1,1},
		{0,0,1,1,0,0},
		{0,0,1,1,0,0},
		{0,0,1,1,0,0},
		{0,0,1,1,0,0}
		},
		{//R
		{1,1,1,1,1,1},
		{1,1,0,0,1,1},
		{1,1,0,0,1,1},
		{1,1,1,1,0,0},
		{1,1,0,0,1,0},
		{1,1,0,0,0,1}
		},
		{//I
		{1,1,0,0,0,0},
		{1,1,0,0,0,0},
		{1,1,0,0,0,0},
		{1,1,0,0,0,0},
		{1,1,0,0,0,0},
		{1,1,0,0,0,0}
		},
		{//S
		{1,1,1,1,1,1},
		{1,1,0,0,0,0},
		{0,1,1,0,0,0},
		{0,0,1,1,0,0},
		{0,0,0,1,1,0},
		{1,1,1,1,1,1}
		}
	};
	for (int i = 0; i < 6; i++) {
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				if (something[i][y][x] == 1) {
					if (i == 0) color = 102;
					else if (i == 1) color = 170;
					else if (i == 2) color = 204;
					else if (i == 3) color = 153;
					else if (i == 4) color = 238;
					else if (i == 5) color = 17;
					consoleBuffer[x + nextword + screen_x * (y + row)].Char.AsciiChar = 'O';
					consoleBuffer[x + nextword + screen_x * (y + row)].Attributes = color;
				}

			}

		}
		if (i == 4) nextword -= 4;
		nextword += 8;
	}
}
void Board() {
	for (int y = 0; y < 30; ++y) {
		for (int x = 2; x < 17; ++x) {
			if ((x > 2 && x < 16) && (y > 0 && y < 29)) continue;
			else if (y == 0) {
				consoleBuffer[x + screen_x * y].Char.AsciiChar = 'G';
				consoleBuffer[x + screen_x * y].Attributes = 119;
			}
			else {
				consoleBuffer[x + screen_x * y].Char.AsciiChar = '#';
				consoleBuffer[x + screen_x * y].Attributes = 119;
			}
		}
	}
}

void ScoreBoard() {
	for (int y = 0; y < 5; ++y) {
		for (int x = 20; x < 40; ++x) {
			if ((x > 20 && x < 39) && (y > 0 && y < 4)) continue;
			consoleBuffer[x + screen_x * y].Char.AsciiChar = '#';
			consoleBuffer[x + screen_x * y].Attributes = 119;
		}
	}
}

void point(long* point) {
	gotoxy(22, 2);
	setcolor(7, 0);
	printf("score = %ld", *point);
}
int level(int deb) {
	char a[9] = "Level : ";
	int j;
	if (deb == 550) j = 1;
	else if (deb == 450) j = 2;
	else if (deb == 350) j = 3;
	else if (deb == 250) j = 4;
	else if (deb == 200) j = 5;
	else if (deb == 150) j = 6;
	else if (deb == 100) j = 7;
	else if (deb == 75) j = 8;
	else if (deb == 50) j = 9;
	for (int y = 7; y < 12; y++) {
		for (int x = 20; x < 40; x++) {
			if ((x > 20 && x < 39) && (y > 7 && y < 11)) continue;
			consoleBuffer[x + screen_x * y].Char.AsciiChar = '#';
			consoleBuffer[x + screen_x * y].Attributes = 119;
		}
	}
	for (int i = 0; i < 8; i++) {
		consoleBuffer[25 + i + screen_x * 9].Char.AsciiChar = a[i];
		consoleBuffer[25 + i + screen_x * 9].Attributes = 7;
	}
	consoleBuffer[32 + screen_x * 9].Char.AsciiChar = j + '0';
	consoleBuffer[32 + screen_x * 9].Attributes = 7;
	return j;
}

int nextlevel(long* point) {
	char a[14] = "Next level in";
	long b = 2500;
	for (int y = 14; y < 21; y++) {
		for (int x = 20; x < 40; x++) {
			if ((x > 20 && x < 39) && (y > 14 && y < 20)) continue;
			consoleBuffer[x + screen_x * y].Char.AsciiChar = '#';
			consoleBuffer[x + screen_x * y].Attributes = 119;
		}
	}for (int i = 0; i < 14; i++) {
		consoleBuffer[23 + i + screen_x * 16].Char.AsciiChar = a[i];
		consoleBuffer[23 + i + screen_x * 16].Attributes = 7;
	}
	if (*point >= 1500000) b = -1;
	else if (*point >= 750000) b = 1500000;
	else if (*point >= 350000) b = 750000;
	else if (*point >= 100000) b = 350000;
	else if (*point >= 50000) b = 100000;
	else if (*point >= 25000) b = 50000;
	else if (*point >= 10000) b = 25000;
	else if (*point >= 2500) b = 10000;
	if(b!=-1) b = b - *point;
	return b;
	
}
void pausemenu(bool* pause, bool* play, bool* startmenuloop,long nl) {
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	for (int y = 22; y < 30; y++) {
		for (int x = 20; x < 40; x++) {
			if ((y > 22 && y < 29) && (x > 20 && x < 39))continue;
			else {
				consoleBuffer[x + screen_x * y].Char.AsciiChar = '#';
				consoleBuffer[x + screen_x * y].Attributes = 119;
			}
		}
	}
	fill_buffer_to_console();
	gotoxy(21, 23);
	setcolor(7, 0);
	printf("------PAUSE-------");
	gotoxy(22, 26);
	printf("RESUME");
	gotoxy(33, 26);
	printf("EXIT");
	while (*pause) {
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);

		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						*pause = false;
						for (int i = 3; i > 0; i--) {
							gotoxy(9, 10);
							setcolor(12, 0);
							printf("%d", i);
							Sleep(1000);
						}
						setcolor(7, 0);
						printf(" ");
					}
				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
						if (posx >= 22 && posx <= 27 && posy == 26) {
							*pause = false;
							for (int i = 3; i > 0; i--) {
								gotoxy(9, 10);
								setcolor(12, 0);
								printf("%d", i);
								Sleep(1000);
							}
							setcolor(7, 0);
							printf(" ");

						}
						else if (posx >= 33 && posx <= 36 && posy == 26) {
							*pause = false;
							*play = false;
							*startmenuloop = false;
						}
					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
						if (posx >= 22 && posx <= 27 && posy == 26) {
							gotoxy(22, 26);
							setcolor(0, 7);
							printf("RESUME");
						}
						else if (posx >= 33 && posx <= 36 && posy == 26) {
							gotoxy(33, 26);
							setcolor(0, 7);
							printf("EXIT");
						}
						else {
							setcolor(7, 0);
							gotoxy(22, 26);
							printf("RESUME");
							gotoxy(33, 26);
							printf("EXIT");
						}
					}

				}
			}
			delete[] eventBuffer;
		}point(&score);
		gotoxy(23, 18);
		setcolor(7, 0);
		if (nl != -1) printf("%ld", nl);
		else if (nl == -1) printf("  LEVEL MAX");
	}
	for (int y = 22; y < 30; y++) {
		for (int x = 20; x < 40; x++) {
			if ((y > 22 && y < 29) && (x > 20 && x < 39))continue;
			else {
				consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
				consoleBuffer[x + screen_x * y].Attributes = 7;
			}
		}
	}
}
int Block(int bnum, int rotation, int x, int y, int d) {
	char dc;
	int colorblock = 0;
	if (d == 0) dc = 'O';
	else if (d == 2) dc = '#';
	else dc = ' ';
	if (bnum == 0) colorblock = 102;
	else if (bnum == 1) colorblock = 170;
	else if (bnum == 2) colorblock = 204;
	else if (bnum == 3) colorblock = 153;
	else if (bnum == 4) colorblock = 238;
	else if (bnum == 5) colorblock = 17;
	else if (bnum == 6) colorblock = 221;
	if (d == 1) colorblock = 0;
	int clearb[4][4] = {
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};
	int block[7][4][4] = {
		{
			//l
			{0,0,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,1,0}
		},
		{
			//S
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,1,0}

		},
		{
			//Z
			{0,0,0,0},
			{0,0,1,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			//I
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},
		{
			//O
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			//J
			{0,0,0,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,1,1,0}
		},
		{
			//T
			{0,0,0,0},
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
	};
	int x1 = 0, y1 = 0;
	for (int check = 0; check < 2; check++) {
		if (rotation == 0) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					x1++;
					if (block[bnum][i][j] == 1) {
						if (check == 1) {
							consoleBuffer[x + x1 + screen_x * (y + y1)].Char.AsciiChar = dc;
							consoleBuffer[x + x1 + screen_x * (y + y1)].Attributes = colorblock;
							if (d == 2) {
								bt[y + y1].px[x + x1] = dc;
								bt[y + y1].cb[x + x1] = colorblock;
							}
						}
						else if (cursor(x + x1, y + y1 + 4) == 'G') {
							if (d == 2) {
								return 2;
							}
						}
						else {
							if (cursor(x + x1, y + y1 + 4) == '#') {
								return 1;

							}
						}
					}
				}
				x1 = 0;
				y1++;
			}
		}
		else if (rotation == 1) {
			for (int j = 0; j < 4; j++) {
				for (int i = 3; i >= 0; i--) {
					x1++;
					if (block[bnum][i][j] == 1) {
						if (check == 1) {
							consoleBuffer[x + x1 + screen_x * (y + y1)].Char.AsciiChar = dc;
							consoleBuffer[x + x1 + screen_x * (y + y1)].Attributes = colorblock;
							if (d == 2) {
								bt[y + y1].px[x + x1] = dc;
								bt[y + y1].cb[x + x1] = colorblock;
							}
						}
						else if (cursor(x + x1, y + y1 + 4) == 'G') {
							if (d == 2) {
								return 2;
							}
						}
						else {
							if (cursor(x + x1, y + y1 + 4) == '#') {
								return 1;
							}
						}
					}
				}
				x1 = 0;
				y1++;
			}
		}
		else if (rotation == 2) {
			for (int i = 3; i >= 0; i--) {
				for (int j = 3; j >= 0; j--) {
					x1++;
					if (block[bnum][i][j] == 1) {
						if (check == 1) {
							consoleBuffer[x + x1 + screen_x * (y + y1)].Char.AsciiChar = dc;
							consoleBuffer[x + x1 + screen_x * (y + y1)].Attributes = colorblock;
							if (d == 2) {
								bt[y + y1].px[x + x1] = dc;
								bt[y + y1].cb[x + x1] = colorblock;
							}
						}
						else if (cursor(x + x1, y + y1 + 4) == 'G') {
							if (d == 2) {
								return 2;
							}
						}
						else {
							if (cursor(x + x1, y + y1 + 4) == '#') {
								return 1;
							}
						}
					}
				}
				x1 = 0;
				y1++;
			}
		}
		else if (rotation == 3) {

			for (int j = 3; j >= 0; j--) {
				for (int i = 0; i < 4; i++) {
					x1++;
					if (block[bnum][i][j] == 1) {
						if (check == 1) {
							consoleBuffer[x + x1 + screen_x * (y + y1)].Char.AsciiChar = dc;
							consoleBuffer[x + x1 + screen_x * (y + y1)].Attributes = colorblock;
							if (d == 2) {
								bt[y + y1].px[x + x1] = dc;
								bt[y + y1].cb[x + x1] = colorblock;
							}
						}
						else if (cursor(x + x1, y + y1 + 4) == 'G') {
							if (d == 2) {
								return 2;
							}
						}
						else {
							if (cursor(x + x1, y + y1 + 4) == '#') {
								return 1;
							}
						}
					}
				}
				x1 = 0;
				y1++;
			}
		}
	}
	return 0;
}

void fonts(int a, int b) {
	int newWidth = a, newHeight = b;
	CONSOLE_FONT_INFOEX fontStructure = { 0 };
	fontStructure.cbSize = sizeof(fontStructure);
	fontStructure.dwFontSize.X = newWidth;
	fontStructure.dwFontSize.Y = newHeight;
	wcscpy(fontStructure.FaceName, L"Consolas");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(hConsole, true, &fontStructure);
}

int blockcheck() {
	int check = 0;
	for (int y = 0; y < 30; ++y) {
		int i = 1;
		for (int x = 3; x < 17; ++x) {
			if ((x >= 3 && x < 16) && (y > 0 && y < 29)) {
				if (cursor(x, y) == '#') continue;
				else {
					i = 0;
					break;
				}
			}
			else continue;
		}
		if (i == 1) {
			check++;
			for (int x = 3; x < 17; ++x) {
				if ((x >= 3 && x < 16) && (y > 0 && y < 29)) {
					//consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
					//consoleBuffer[x + screen_x * y].Attributes = 7;
					bt[y].px[x] = ' ';
					bt[y].cb[x] = 7;
				}
				else continue;
			}
		}
	}
	return check;
}

void BlockTransition() {
	int count = 0;
	for (int y = 28; y >= 1; y--) {
		count = 0;
		int i = 1;
		for (int x = 3; x < 16; ++x) {
			if ((x >= 3 && x < 16) && (y > 0 && y < 29)) {
				if (bt[y].px[x] == ' ') continue;
				else {
					i = 0;
					break;
				}
			}
			else continue;
		}
		if (i == 1) {
			++count;
			for (int j = 1; j < 4; j++) {
				int check = 0;
				for (int x = 3; x < 16; x++) {
					if (bt[y - j].px[x] == ' ') {
						check++;
					}
				}
				if (check == 13) {
					count++;
				}
				else break;
			}
			/*setcolor(7, 0);
			gotoxy(10, 10);
			printf("%d", count);
			Sleep(5000);*/
		}
		for (int y1 = y; y1 >= 1; y1--) {
			for (int x = 3; x < 16; ++x) {
				if (y1 - count >= 1) {
					bt[y1].px[x] = bt[y1 - count].px[x];
					bt[y1].cb[x] = bt[y1 - count].cb[x];
				}
				else {
					bt[y1].px[x] = ' ';
					bt[y1].cb[x] = 7;
				}
			}
		}
		for (int x = 3; x < 17; ++x) {
			if ((x >= 3 && x < 16) && (y > 0 && y < 29)) {
				consoleBuffer[x + screen_x * y].Char.AsciiChar = bt[y].px[x];
				consoleBuffer[x + screen_x * y].Attributes = bt[y].cb[x];
			}
			else continue;
		}
	}
}

void gameoverdisplay(bool* gameover) {
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	clear_buffer();
	fill_buffer_to_console();

	int GAMEOVER[8][6][6]{
		{//G
		{1,1,1,1,1,1},
		{1,1,1,1,1,1},
		{1,1,0,0,0,0},
		{1,1,0,1,1,1},
		{1,1,0,0,1,1},
		{1,1,1,1,1,1}
		},
		{//A
		{1,1,1,1,1,1},
		{1,1,0,0,1,1},
		{1,1,1,1,1,1},
		{1,1,1,1,1,1},
		{1,1,0,0,1,1},
		{1,1,0,0,1,1}
		},
		{//M
		{1,1,1,1,1,1},
		{1,1,1,1,1,1},
		{1,0,1,1,0,1},
		{1,0,1,1,0,1},
		{1,0,1,1,0,1},
		{1,0,1,1,0,1}
		},
		{//E
		{1,1,1,1,1,1},
		{1,1,0,0,0,0},
		{1,1,1,1,1,1},
		{1,1,1,1,1,1},
		{1,1,0,0,0,0},
		{1,1,1,1,1,1}
		},
		{//O
		{1,1,1,1,1,1},
		{1,1,0,0,1,1},
		{1,1,0,0,1,1},
		{1,1,0,0,1,1},
		{1,1,0,0,1,1},
		{1,1,1,1,1,1}
		},
		{//V
		{1,0,0,0,0,1},
		{1,1,0,0,1,1},
		{0,1,0,0,1,0},
		{0,1,0,0,1,0},
		{0,0,1,1,0,0},
		{0,0,1,1,0,0}
		},
		{//E
		{1,1,1,1,1,1},
		{1,1,0,0,0,0},
		{1,1,1,1,1,1},
		{1,1,1,1,1,1},
		{1,1,0,0,0,0},
		{1,1,1,1,1,1}
		},
		{//R
		{1,1,1,1,1,1},
		{1,1,0,0,1,1},
		{1,1,0,0,1,1},
		{1,1,1,1,0,0},
		{1,1,0,0,1,0},
		{1,1,0,0,0,1}
		}
	};
	int ic = 0;
	int icc[2] = { 119,255 };
	int deb = 500;

	while (*gameover) {
		if (deb == 500) {
			int nextword = 5;
			int nextbar = 0;
			for (int i = 0; i < 8; i++) {
				if (i == 4) {
					nextbar += 8;
					nextword = 5;
				}
				for (int y = 0; y < 6; y++) {
					for (int x = 0; x < 6; x++) {
						if (GAMEOVER[i][y][x] == 1) {
							consoleBuffer[x + 1 + nextword + screen_x * (y + 5 + 2 + nextbar)].Char.AsciiChar = 'O';
							consoleBuffer[x + 1 + nextword + screen_x * (y + 5 + 2 + nextbar)].Attributes = icc[ic];
						}
					}
				}
				nextword += 8;
			}
			fill_buffer_to_console();
			ic = (ic + 1) % 2;
			deb = 0;
			gotoxy(11, 23);
			setcolor(7, 0);
			printf("Your score is : %d", score);
			gotoxy(12,25);
			printf("Press esc to menu");
		}
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {

					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						*gameover = false;

					}

				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						FROM_LEFT_1ST_BUTTON_PRESSED) {
					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						RIGHTMOST_BUTTON_PRESSED) {
					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
					}
				}
			}
			delete[] eventBuffer;
		}
		Sleep(50);
		deb += 50;
	}
}

void leaderboard() {
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;

	clear_buffer();
	fill_buffer_to_console();
	setcolor(7, 0);
	gotoxy(17, 5);
	printf("LEADERBOARD");
	bool leader = true;
	FILE* fp = fopen("Leaderscore.txt", "r");
	int c = 0;
	gotoxy(7, 8);
	printf("Name		     	  Score");
	for (int i = 1; i < 6; i++) {
		char a[20];
		long b;
		fscanf(fp, "%s %ld", a, &b);
		long d = b;
		int count = 0;
		gotoxy(4, 10 + c);
		if (b == -1) printf("%d. -			    -", i);
		else {
			while (d != 0) {
				d = d / 10;
				count += 1;
			}
			printf("%d. %s", i, a);
			gotoxy(39 - count, 10 + c);
			printf("%ld\n\n", b);
		}
		c += 3;
	}
	gotoxy(4, 28);
	printf("Back");
	while (leader) {
		setcolor(7, 0);
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						leader = false;
					}
				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
						if (posx >= 4 && posx <= 7 && posy == 28) leader = false;
					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						RIGHTMOST_BUTTON_PRESSED) {
					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {

					}
					if (posx >= 4 && posx <= 7 && posy == 28) {
						gotoxy(4, 28);
						setcolor(0, 7);
						printf("Back");
					}
					else {
						gotoxy(4, 28);
						setcolor(7, 0);
						printf("Back");
					}
				}
			}
			delete[] eventBuffer;
		}
	}
}
void checkleader() {
	FILE* fp;
	fp = fopen("Leaderscore.txt", "r");
	if (fp == NULL) {
		fp = fopen("Leaderscore.txt", "w");
		for (int i = 0; i < 5; i++) {
			long a = -1;
			fprintf(fp, "NULL %ld\n", a);
		}
		fclose(fp);
	}
	else {
		fclose(fp);
	}
}
int main() {

	bool start = true;
	bool play = false;
	bool startmenu = true;
	bool startmenuloop = false;
	bool pause = false;
	checkleader();
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setConsole(screen_x, screen_y);
	setMode();
	srand(time(NULL));
	BlockNum = rand() % 7;
	int colorship = 7;
	int xBlock = 5 + rand() % 7;
	int yBlock = -9;
	while (start) {
		setcursor(0);
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (startmenu == true && startmenuloop == false) {
			startmenuloop = true;
			fonts(20, 20);
			clear_buffer();
			fill_buffer_to_console();
			TETRIS();
			fill_buffer_to_console();
			gotoxy(20, 15);
			setcolor(7, 0);
			printf("START");
			gotoxy(17, 20);
			printf("LEADERBOARD");
			gotoxy(21, 25);
			printf("EXIT");
			gotoxy(2, 33);
			printf("Saranthorn Pornchaiwattanakorn  64010827");
		}
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						start = false;
					}
				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						FROM_LEFT_1ST_BUTTON_PRESSED) {
						if ((posx >= 20 && posx <= 25) && posy == 15) {
							play = true;
						}
						else if ((posx >= 17 && posx <= 27) && posy == 20) {
							leaderboard();
							startmenuloop = false;
						}
						else if ((posx >= 21 && posx <= 24) && posy == 25) {
							start = false;
						}
					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						RIGHTMOST_BUTTON_PRESSED) {
					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
						if ((posx >= 20 && posx <= 25) && posy == 15) {
							gotoxy(20, 15);
							setcolor(0, 7);
							printf("START");
						}
						else if ((posx >= 17 && posx <= 27) && posy == 20) {
							gotoxy(17, 20);
							setcolor(0, 7);
							printf("LEADERBOARD");
						}
						else if ((posx >= 21 && posx <= 24) && posy == 25) {
							gotoxy(21, 25);
							setcolor(0, 7);
							printf("EXIT");
						}
						else {
							gotoxy(20, 15);
							setcolor(7, 0);
							printf("START");
							gotoxy(17, 20);
							setcolor(7, 0);
							printf("LEADERBOARD");
							gotoxy(21, 25);
							setcolor(7, 0);
							printf("EXIT");
						}
					}
				}
			}
			delete[] eventBuffer;
		}
		if (play == true) {
			clear_buffer();
			fill_buffer_to_console();
			char playername[100];
			setcursor(1);
			setcolor(7, 0);
			gotoxy(10, 12);
			printf("Enter your player name\n\n\n\n");
			gotoxy(9, 16);
			printf("|			|");
			gotoxy(7, 20);
			printf("Press Enter to start the game");
			gotoxy(10, 16);
			scanf("%s", playername);
			setcursor(0);
			//fonts(10, 10);
			for (int yi = 0; yi < 32; yi++) {
				for (int xi = 0; xi < 17; xi++) {
					bt[yi].px[xi] = ' ';
					bt[yi].cb[xi] = 7;
				}
			}
			xBlock = 5 + rand() % 7;
			yBlock = -9;
			rotation = 0;
			BlockNum = rand() % 7;
			score = 0;
			clear_buffer();
			fill_buffer_to_console();
			Board();
			ScoreBoard();
			fill_buffer_to_console();
			point(&score);
			int countendgame = 0;
			int deb = 300;
			int deblevel = 550;
			while (play) {
				gotoxy(20, 20);
				GetNumberOfConsoleInputEvents(rHnd, &numEvents);
				if (score >= 2500 && deblevel == 550) deblevel -= 100;
				else if (score >= 10000 && deblevel == 450) deblevel -= 100;
				else if (score >= 25000 && deblevel == 350) deblevel -= 100;
				else if (score >= 50000 && deblevel == 250) deblevel -= 50;
				else if (score >= 100000 && deblevel == 200) deblevel -= 50;
				else if (score >= 350000 && deblevel == 150) deblevel -= 50;
				else if (score >= 750000 && deblevel == 100) deblevel -= 25;
				else if (score >= 1500000 && deblevel == 75) deblevel -= 25;
				int le = level(deblevel);
				long nl = nextlevel(&score);
				
				if (deb >= deblevel) {
					deb = 0;
					Block(BlockNum, rotation, xBlock, yBlock, 1);
					yBlock += 1;
				}
				if (Block(BlockNum, rotation, xBlock, yBlock, 0) == 1) {
					yBlock -= 1;
					Block(BlockNum, rotation, xBlock, yBlock, 2);
					if (Block(BlockNum, rotation, xBlock, yBlock, 2) == 2) {
						bool gameover = true;
						if (gameover == true) {
							FILE* fp;
							char player[5][20];
							long sc[5];
							int i = 0;
							char temp[15];
							long stemp;
							for (int k = 15; k < 100; k++) {
								playername[k] = '\0';
							}
							i = 0;
							fp = fopen("Leaderscore.txt", "r");
							while (!feof(fp)) {
								fscanf(fp, "%s %ld", player[i], &sc[i]);
								i++;
							}
							for (i = 0; i < 5; i++) {
								if (score > sc[i]) {
									strcpy(temp, player[i]);
									stemp = sc[i];
									strcpy(player[i], playername);
									sc[i] = score;
									break;
								}
							}
							for (int j = i + 1; j < 5; j++) {
								char tempj[15];
								long stempj;
								strcpy(tempj, player[j]);
								stempj = sc[j];
								strcpy(player[j], temp);
								sc[j] = stemp;
								strcpy(temp, tempj);
								stemp = stempj;
							}
							fclose(fp);
							fp = fopen("Leaderscore.txt", "w");
							for (int i = 0; i < 5; i++) {
								fprintf(fp, "%s %ld\n", player[i], sc[i]);
							}
							fclose(fp);
						}
						gameoverdisplay(&gameover);
						play = false;
						startmenuloop = false;
						clear_buffer();
						fill_buffer_to_console();
						if (startmenuloop == false) break;
					}
					Board();
					ScoreBoard();
					fill_buffer_to_console();
					int bcc = blockcheck();
					if (bcc - 2 > 0) {
						if (bcc - 2 == 1) { std::thread q(Beep, 225, 250); q.detach(); }
						else if (bcc - 2 == 2) { std::thread q(Beep, 250, 250); q.detach(); }
						else if (bcc - 2 == 3) { std::thread q(Beep, 275, 250); q.detach(); }
						else if (bcc - 2 == 4) { std::thread q(Beep, 300, 250); q.detach(); }
						fill_buffer_to_console();
						score = score + ((bcc - 2) * (bcc - 2) * 150) + (bcc - 2) * 250 * le;
						BlockTransition();
						fill_buffer_to_console();
						/*for (int yi = 1; yi < 29; yi++) {
							for (int xi = 3; xi < 16; xi++) {
								consoleBuffer[xi + screen_x * yi].Char.AsciiChar = bt[yi].px[xi];
								consoleBuffer[xi + screen_x * yi].Attributes = bt[yi].cb[xi];
							}
						}*/
					}
					xBlock = 5 + rand() % 7;
					yBlock = -9;
					rotation = 0;
					BlockNum = rand() % 7;

				}
				else {
					Block(BlockNum, rotation, xBlock, yBlock, 0);
					Board();
					ScoreBoard();
					fill_buffer_to_console();
				}

				if (numEvents != 0) {

					INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
					ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
					for (DWORD i = 0; i < numEventsRead; ++i) {
						if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
							
							if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
								pause = true;
								pausemenu(&pause, &play, &startmenuloop,nl);
								if (pause == false) {

								}
							}
							else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == 77 ) {
								score += 10000;
							}
							else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
								int cc = xBlock;
								Block(BlockNum, rotation, xBlock, yBlock, 1);
								rotation += 1;
								if (rotation == 4) rotation = 0;
								int countloop = 0;
								while (Block(BlockNum, rotation, xBlock, yBlock, 1) == 1) {
									countloop++;
									if (Block(BlockNum, rotation, xBlock + 1, yBlock, 1) == 1 && Block(BlockNum, rotation, xBlock - 1, yBlock, 1) == 1) rotation -= 1;
									else {
										if (cc > 10) {
											--xBlock;
											if (countloop > 2) {
												rotation -= 1;
												xBlock = xBlock + countloop;
												break;
											}
										}
										else if(cc<3){
											++xBlock;
											countloop++;
											if (countloop > 2) {
												rotation -= 1;
												xBlock = xBlock - countloop;
												break;
											}
										}
										else {
											bool trytodosomething = true;
											bool trytomakesomething = true;
											if (trytodosomething == true) {
												xBlock += 1;
												if (Block(BlockNum, rotation, xBlock, yBlock, 1) == 0) {
													trytodosomething = false;
													trytomakesomething = false;
													break;
												}
												else xBlock -= 1;
											}
											else if (trytomakesomething) {
												xBlock -= 1;
												if (Block(BlockNum, rotation, xBlock, yBlock, 1) == 0) {
													trytodosomething = false;
													trytomakesomething = false;
													break;
												}
												else xBlock += 1;
											}
										}
									}
									if (countloop > 2) {
										rotation -= 1;
										break;
									}
									if (rotation == -1) rotation = 3;
								}
							}
							else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == KEY_RIGHT && yBlock > -6) {
								Block(BlockNum, rotation, xBlock, yBlock, 1);
								xBlock += 1;
								if (Block(BlockNum, rotation, xBlock, yBlock, 0) == 1) {
									xBlock -= 1;
								}
							}
							else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == KEY_LEFT && yBlock > -6) {
								Block(BlockNum, rotation, xBlock, yBlock, 1);
								xBlock -= 1;
								if (Block(BlockNum, rotation, xBlock, yBlock, 0) == 1) {
									xBlock += 1;
								}
							}
							else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == KEY_DOWN) {
								deb += deblevel;
								score += 1;
							}
						}
						else if (eventBuffer[i].EventType == MOUSE_EVENT) {
							int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
							int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
							if (eventBuffer[i].Event.MouseEvent.dwButtonState &
								FROM_LEFT_1ST_BUTTON_PRESSED) {
							}
							else if (eventBuffer[i].Event.MouseEvent.dwButtonState &
								RIGHTMOST_BUTTON_PRESSED) {
							}
							else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
							}
						}

					}

					delete[] eventBuffer;
				}
				deb += 50;
				point(&score);
				gotoxy(23, 18);
				setcolor(7, 0);
				if (nl != -1) printf("%ld", nl);
				else if (nl == -1) printf("  LEVEL MAX");
				Sleep(50);
				setcolor(7, 0);
			}
		}
	}
	return 0;
}
