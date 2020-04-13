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
	
	struct Tetromino {
		int row;
		int col;
		int mType;
		vector<vector<int>> mTetromino;

		Tetromino();
		void reset();
	} currentTetromino;

	vector<vector<int>> I_MINO = { {0, 0, 0, 0}, {TETROMINO_I, TETROMINO_I, TETROMINO_I, TETROMINO_I}, {0, 0, 0, 0}, {0, 0, 0, 0} };
	vector<vector<int>> O_MINO = { {0, TETROMINO_O, TETROMINO_O, 0}, {0, TETROMINO_O, TETROMINO_O, 0} };
	vector<vector<int>> T_MINO = { {0, TETROMINO_T, 0}, {TETROMINO_T, TETROMINO_T, TETROMINO_T}, {0, 0, 0} };
	vector<vector<int>> S_MINO = { {0, TETROMINO_S, TETROMINO_S}, {TETROMINO_S, TETROMINO_S, 0}, {0, 0, 0} };
	vector<vector<int>> Z_MINO = { {TETROMINO_Z, TETROMINO_Z, 0}, {0, TETROMINO_Z, TETROMINO_Z}, {0, 0, 0} };
	vector<vector<int>> J_MINO = { {TETROMINO_J, 0, 0}, {TETROMINO_J, TETROMINO_J, TETROMINO_J}, {0, 0, 0} };
	vector<vector<int>> L_MINO = { {0, 0, TETROMINO_L}, {TETROMINO_L, TETROMINO_L, TETROMINO_L}, {0, 0, 0} };

	void generateNextTetrominoSet();
	void updateScreen();
	void checkAndGenerateTetrominoSet();
	void checkAndRefillTetrominoQueue();
	void spawn();
	void writeToBoard(vector<vector<int>> tetromino, bool clear = false);
	void tick();
	bool willOverlap();

public:
	Game();
	~Game();

	bool initialize();
	void run();
	void close();
	void printBoard();
};