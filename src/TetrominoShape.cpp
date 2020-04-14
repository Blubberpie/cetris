#include "include/TetrominoShape.hpp"

vector<vector<int>> TetrominoShape::getTetrominoShape(int tetrominoType) {
	switch (tetrominoType) {
	case TETROMINO_I:
		return I_MINO;
	case TETROMINO_O:
		return O_MINO;
	case TETROMINO_T:
		return T_MINO;
	case TETROMINO_S:
		return S_MINO;
	case TETROMINO_Z:
		return Z_MINO;
	case TETROMINO_J:
		return J_MINO;
	case TETROMINO_L:
		return L_MINO;
	default:
		return {};
	}
}