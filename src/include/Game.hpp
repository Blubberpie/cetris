#pragma once
#include "Renderer.hpp"
#include "constants.hpp"
#include <queue>
#include <iostream>
#include <ctime>

using namespace std;
using namespace constants;

class Game
{
private:
	Renderer renderer;
	int gameBoard[NUM_ROWS][NUM_COLS] = {};
	queue<int> tetrominoGenerated;
	queue<int> tetrominoNext;
	int lastHighestRow = NUM_ROWS - 1;
	
	struct Tetromino {
		int row;
		int col;
		int mType;
		int orientation;
		vector<vector<int>> mTetromino;

		Tetromino();
		void reset();
	} currentTetromino;

	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		numDirections
	};

	// Tetromino shapes
	vector<vector<int>> I_MINO = { {0, 0, 0, 0}, {TETROMINO_I, TETROMINO_I, TETROMINO_I, TETROMINO_I}, {0, 0, 0, 0}, {0, 0, 0, 0} };
	vector<vector<int>> O_MINO = { {0, TETROMINO_O, TETROMINO_O, 0}, {0, TETROMINO_O, TETROMINO_O, 0} };
	vector<vector<int>> T_MINO = { {0, TETROMINO_T, 0}, {TETROMINO_T, TETROMINO_T, TETROMINO_T}, {0, 0, 0} };
	vector<vector<int>> S_MINO = { {0, TETROMINO_S, TETROMINO_S}, {TETROMINO_S, TETROMINO_S, 0}, {0, 0, 0} };
	vector<vector<int>> Z_MINO = { {TETROMINO_Z, TETROMINO_Z, 0}, {0, TETROMINO_Z, TETROMINO_Z}, {0, 0, 0} };
	vector<vector<int>> J_MINO = { {TETROMINO_J, 0, 0}, {TETROMINO_J, TETROMINO_J, TETROMINO_J}, {0, 0, 0} };
	vector<vector<int>> L_MINO = { {0, 0, TETROMINO_L}, {TETROMINO_L, TETROMINO_L, TETROMINO_L}, {0, 0, 0} };

	
	// Kick tables based on https://tetris.wiki/Super_Rotation_System
	int KICK_TABLE_UP_RIGHT[4][2] = { {0, -1}, {-1, -1}, {2, 0}, {2, -1} }; // UP -> RIGHT = DOWN -> RIGHT
	int KICK_TABLE_RIGHT_DOWN[4][2] = { {0, 1}, {1, 1}, {-2, 0}, {-2, 1} }; // RIGHT -> DOWN = RIGHT -> UP
	int KICK_TABLE_DOWN_LEFT[4][2] = { {0, 1}, {-1, 1}, {2, 0}, {2, 1} }; // DOWN -> LEFT = UP -> LEFT
	int KICK_TABLE_LEFT_UP[4][2] = { {0, -1}, {1, -1}, {-2, 0}, {-2, -1} }; // LEFT -> UP = LEFT -> DOWN

	int KICK_TABLE_I_UP_RIGHT[4][2] = { {0, -2}, {0, 1}, {1, -2}, {-2, 1} }; // UP -> RIGHT = LEFT -> DOWN
	int KICK_TABLE_I_RIGHT_DOWN[4][2] = { {0, -1}, {0, 2}, {-2, -1}, {1, 2} }; // RIGHT -> DOWN = UP -> LEFT
	int KICK_TABLE_I_DOWN_LEFT[4][2] = { {0, 2}, {0, -1}, {-1, 2}, {2, -1} }; // DOWN -> LEFT = RIGHT -> UP
	int KICK_TABLE_I_LEFT_UP[4][2] = { {0, 1}, {0, -2}, {2, 1}, {-1, -2} }; // LEFT -> UP = DOWN -> RIGHT

	void generateNextTetrominoSet();
	void updateScreen();
	void checkAndGenerateTetrominoSet();
	void checkAndRefillTetrominoQueue();
	void spawn();
	void writeToBoard(vector<vector<int>> &tetromino, bool clear = false);
	void tick();
	void move(int direction);
	void rotate(int direction);
	bool passedKickTest(vector<vector<int>> &tetromino, int direction);
	bool performKickTests(vector<vector<int>>& tetromino, int kickTable[4][2]);
	bool willCollide(vector<vector<int>>& tetromino, int startRow, int startCol);
	void transposeLeft(vector<vector<int>> &tetromino);
	void transposeRight(vector<vector<int>> &tetromino);
	void reverseColumns(vector<vector<int>> &tetromino);
	void clearLines();
	void cascade(int endRow);

public:
	Game();
	~Game();

	bool initialize();
	void run();
	void close();
	void printBoard();
};