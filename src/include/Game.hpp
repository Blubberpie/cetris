#pragma once
#include "Renderer.hpp"
#include "constants.hpp"
#include "TetrominoShape.hpp"
#include <iostream>
#include <ctime>
#include <queue>

using namespace std;
using namespace constants;

class Game
{
private:
	Renderer renderer;
	int gameBoard[NUM_ROWS][NUM_COLS] = {};
	int lastHighestRow = NUM_ROWS - 1;
	int holdType = EMPTY_TILE;
	bool alreadyHeld = false;
	bool dropped = false;
	bool gameOver = false;
	bool quit = false;
	SDL_Event event;
	const Uint8* keyboardStates;
	int currentMenu;
	int currentLevel = 1;
	int currentScore = 0;

	queue<int> tetrominoGenerated;
	queue<int> tetrominoNext;

	Uint32 currentTick = 0;
	Uint32 lastTick = 0;
	Uint32 lastLockDelay = 0;

	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		numDirections
	};

	enum MenuType {
		MAIN_MENU,
		GAME,
		SETTINGS,
		numMenus
	};

	struct Tetromino {
		int row;
		int col;
		int ghostRow;
		int mType;
		int orientation;
		vector<vector<int>> mTetromino;

		Tetromino();
		void reset();
	} currentTetromino;

	struct KeyboardKey {
		Direction direction;
		bool alreadyPressed = false;
		int delay = 0;
		int repeatRate = 0;
		Uint32 lastRepeatTime = 0;
		Uint32 lastDelayTime = 0;
	};

	KeyboardKey downKey;
	KeyboardKey leftKey;
	KeyboardKey rightKey;
	
	// SRS kick tables based on https://tetris.wiki/Super_Rotation_System
	int KICK_TABLE_UP_RIGHT[4][2] = { {0, -1}, {-1, -1}, {2, 0}, {2, -1} }; // UP -> RIGHT & DOWN -> RIGHT
	int KICK_TABLE_RIGHT_DOWN[4][2] = { {0, 1}, {1, 1}, {-2, 0}, {-2, 1} }; // RIGHT -> DOWN & RIGHT -> UP
	int KICK_TABLE_DOWN_LEFT[4][2] = { {0, 1}, {-1, 1}, {2, 0}, {2, 1} }; // DOWN -> LEFT & UP -> LEFT
	int KICK_TABLE_LEFT_UP[4][2] = { {0, -1}, {1, -1}, {-2, 0}, {-2, -1} }; // LEFT -> UP & LEFT -> DOWN

	int KICK_TABLE_I_UP_RIGHT[4][2] = { {0, -2}, {0, 1}, {1, -2}, {-2, 1} }; // UP -> RIGHT & LEFT -> DOWN
	int KICK_TABLE_I_RIGHT_DOWN[4][2] = { {0, -1}, {0, 2}, {-2, -1}, {1, 2} }; // RIGHT -> DOWN & UP -> LEFT
	int KICK_TABLE_I_DOWN_LEFT[4][2] = { {0, 2}, {0, -1}, {-1, 2}, {2, -1} }; // DOWN -> LEFT & RIGHT -> UP
	int KICK_TABLE_I_LEFT_UP[4][2] = { {0, 1}, {0, -2}, {2, 1}, {-1, -2} }; // LEFT -> UP & DOWN -> RIGHT

	void runMenu();
	void runCetris();
	void generateNextTetrominoSet();
	void updateMainGameScreen();
	void updateMainMenuScreen();
	void checkAndGenerateTetrominoSet();
	void checkAndRefillTetrominoQueue();
	void spawn();
	void writeToBoard(vector<vector<int>> &tetromino, bool clear = false);
	void tick();
	bool move(int direction);
	void handleKeyPress(KeyboardKey& key);
	int calculateDrop();
	void hardDrop();
	void rotate(int direction);
	bool passedKickTest(vector<vector<int>>& tetromino, int direction);
	bool performKickTests(vector<vector<int>>& tetromino, int kickTable[4][2]);
	bool willCollide(vector<vector<int>>& tetromino, int startRow, int startCol);
	void transposeLeft(vector<vector<int>>& tetromino);
	void transposeRight(vector<vector<int>>& tetromino);
	void reverseColumns(vector<vector<int>>& tetromino);
	void clearLines();
	void cascade(int endRow);
	void hold();
	void resetDirections();
	void lockTetromino();
	void resetGame();
	void calculateLineClearScore(int linesCleared);

public:
	Game();
	~Game();

	bool initialize();
	void run();
	void close();
	void printBoard();
};