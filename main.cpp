#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include<string.h>
#define screen_x 50
#define screen_y 35
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

int score = 0;
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
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
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
	int nextword = 4;
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
void point(int* point) {
	gotoxy(22, 2);
	setcolor(7, 0);
	printf("score = %d", *point);
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
	wcscpy(fontStructure.FaceName, L"Arial");
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
					consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
					consoleBuffer[x + screen_x * y].Attributes = 7;
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
	for (int y = 28; y >= 0; y--) {
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
		}
		for (int y1 = y; y1 >= 1; y1--) {
			for (int j = 1; j < 4; j++) {
				for (int x = 3; x < 16; x++) {
					if (bt[y1 - j].px[x] == ' ' && y1 - j >= 0) {
						if (x == 15) {
							count++;
						}continue;
					}
					else {
						j = 5;
						break;
					}
				}
			}
			for (int x = 3; x < 16; ++x) {
				bt[y1].px[x] = bt[y1 - count].px[x];
				bt[y1].cb[x] = bt[y1 - count].cb[x];
			}
		}
		for (int x = 3; x < 17; ++x) {
			if ((x >= 3 && x < 16) && (y > 0 && y < 29)) {
				if (y - count >= 0) {
					consoleBuffer[x + screen_x * y].Char.AsciiChar = bt[y].px[x];
					consoleBuffer[x + screen_x * y].Attributes = bt[y].cb[x];
				}
			}
			else continue;
		}
	}
}

int main() {
	bool start = true;
	bool play = false;
	bool startmenu = true;
	bool startmenuloop = false;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setConsole(screen_x, screen_y);
	setMode();
	srand(time(NULL));
	setcursor(0);
	BlockNum = rand() % 7;
	int colorship = 7;
	int xBlock = 5 + rand() % 7;
	int yBlock = -9;

	while (start) {
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (startmenu == true && startmenuloop == false) {
			startmenuloop = true;
			fonts(10, 15);
			clear_buffer();
			fill_buffer_to_console();
			TETRIS();
			fill_buffer_to_console();
			gotoxy(23, 15);
			setcolor(7, 0);
			printf("START");
			gotoxy(20, 20);
			setcolor(7, 0);
			printf("LEADERBOARD");
			gotoxy(24, 25);
			setcolor(7, 0);
			printf("EXIT");
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
						if ((posx >= 23 && posx <= 27) && posy == 15) {
							play = true;
						}
						else if ((posx >= 24 && posx <= 30) && posy == 20) {

						}
						else if ((posx >= 20 && posx <= 27) && posy == 25) {
							start = false;
						}
					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						RIGHTMOST_BUTTON_PRESSED) {
					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
						if ((posx >= 23 && posx <= 27) && posy == 15) {
							gotoxy(23, 15);
							setcolor(0, 7);
							printf("START");
						}
						else if ((posx >= 24 && posx <= 30) && posy == 20) {
							gotoxy(20, 20);
							setcolor(0, 7);
							printf("LEADERBOARD");
						}
						else if ((posx >= 20 && posx <= 27) && posy == 25) {
							gotoxy(24, 25);
							setcolor(0, 7);
							printf("EXIT");
						}
						else {
							gotoxy(23, 15);
							setcolor(7, 0);
							printf("START");
							gotoxy(20, 20);
							setcolor(7, 0);
							printf("LEADERBOARD");
							gotoxy(24, 25);
							setcolor(7, 0);
							printf("EXIT");
						}
					}
				}
			}
			delete[] eventBuffer;
		}
		if (play == true) {
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
			while (play) {
				GetNumberOfConsoleInputEvents(rHnd, &numEvents);
				if (deb >= 300) {
					deb = 0;
					Block(BlockNum, rotation, xBlock, yBlock, 1);
					yBlock += 1;
				}
				if (Block(BlockNum, rotation, xBlock, yBlock, 0) == 1) {
					yBlock -= 1;
					Block(BlockNum, rotation, xBlock, yBlock, 2);
					if (Block(BlockNum, rotation, xBlock, yBlock, 2) == 2) {
						bool gameover = true;
						clear_buffer();
						fill_buffer_to_console();
						while (gameover) {
							GetNumberOfConsoleInputEvents(rHnd, &numEvents);
							gotoxy(23, 10);
							setcolor(7, 0);
							printf("game over");
							if (numEvents != 0) {

								INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
								ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
								for (DWORD i = 0; i < numEventsRead; ++i) {
									if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
										int countloop = 0;
										if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
											play = false;
											startmenuloop = false;
											gameover = false;
											clear_buffer();
											fill_buffer_to_console();
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
						}
						if (startmenuloop == false) break;
					}
					Board();
					ScoreBoard();
					fill_buffer_to_console();
					int bcc = blockcheck();
					if (bcc - 2 > 0) {
						fill_buffer_to_console();
						score = score + ((bcc - 2) * 250);
						BlockTransition();
						fill_buffer_to_console();

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
							int countloop = 0;
							if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
								play = false;
								startmenuloop = false;
								clear_buffer();
								fill_buffer_to_console();
							}
							else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == 99 || eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == 67) {

							}
							else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
								int cc = xBlock;
								Block(BlockNum, rotation, xBlock, yBlock, 1);
								rotation += 1;
								if (rotation == 4) rotation = 0;
								while (Block(BlockNum, rotation, xBlock, yBlock, 1) == 1) {
									if (Block(BlockNum, rotation, xBlock + 1, yBlock, 1) == 1 && Block(BlockNum, rotation, xBlock - 1, yBlock, 1) == 1) rotation -= 1;
									else {
										if (cc > 10) {
											--xBlock;
											countloop++;
										}
										else {
											++xBlock;
											countloop--;
										}
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
								deb += 300;
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
						point(&score);
					}

					delete[] eventBuffer;
				}
				deb += 50;
				point(&score);
				Sleep(50);
				setcolor(7, 0);
			}
		}
	}
	return 0;
}
