#pragma once
#include "TetrominoTypes.hpp"
#include <iostream>
#include <vector>

using namespace std;

class TetrominoShape
{
private:
	// Tetromino shapes based on https://tetris.wiki/File:SRS-pieces.png
	static inline vector<vector<int>> I_MINO = { {0, 0, 0, 0}, {TETROMINO_I, TETROMINO_I, TETROMINO_I, TETROMINO_I}, {0, 0, 0, 0}, {0, 0, 0, 0} };
	static inline vector<vector<int>> O_MINO = { {0, TETROMINO_O, TETROMINO_O, 0}, {0, TETROMINO_O, TETROMINO_O, 0} };
	static inline vector<vector<int>> T_MINO = { {0, TETROMINO_T, 0}, {TETROMINO_T, TETROMINO_T, TETROMINO_T}, {0, 0, 0} };
	static inline vector<vector<int>> S_MINO = { {0, TETROMINO_S, TETROMINO_S}, {TETROMINO_S, TETROMINO_S, 0}, {0, 0, 0} };
	static inline vector<vector<int>> Z_MINO = { {TETROMINO_Z, TETROMINO_Z, 0}, {0, TETROMINO_Z, TETROMINO_Z}, {0, 0, 0} };
	static inline vector<vector<int>> J_MINO = { {TETROMINO_J, 0, 0}, {TETROMINO_J, TETROMINO_J, TETROMINO_J}, {0, 0, 0} };
	static inline vector<vector<int>> L_MINO = { {0, 0, TETROMINO_L}, {TETROMINO_L, TETROMINO_L, TETROMINO_L}, {0, 0, 0} };

public:
	static vector<vector<int>> getTetrominoShape(int tetrominoType);
};

