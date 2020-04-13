#include "Game.hpp"

Game::Game() {
}

Game::~Game() {
}

bool Game::initialize() {
	bool success = true;
	if (!renderer.initialize(GAME_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT)) { success = false; } 
	else if (!renderer.loadMedia()) { success = false; }
	return success; 
}

void Game::run() {
	srand((unsigned int)time(nullptr));
	bool quit = false;
	SDL_Event event;
	checkAndGenerateTetrominoSet();
	checkAndRefillTetrominoQueue();
	spawn();
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			else if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym) {
				case SDLK_w:
					break;
				case SDLK_a:
					move(LEFT);
					break;
				case SDLK_s:
					move(DOWN);
					break;
				case SDLK_d:
					move(RIGHT);
					break;
				case SDLK_DOWN:
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_q:
					break;
				}
			}
		}
		checkAndRefillTetrominoQueue();
		checkAndGenerateTetrominoSet();
		updateScreen();
	}
}

// Based on BPS's Random Generator
void Game::generateNextTetrominoSet() {
	int tetrominos[] = { 1, 2, 3, 4, 5, 6, 7 };
	for (int i = NUM_TETROMINO - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(tetrominos[i], tetrominos[j]);
		tetrominoGenerated.push(tetrominos[i]);
	}
	tetrominoGenerated.push(tetrominos[0]);
}

void Game::checkAndGenerateTetrominoSet() {
	while (tetrominoGenerated.size() < NUM_TETROMINO) {
		generateNextTetrominoSet();
	}
}

void Game::checkAndRefillTetrominoQueue() {
	while (tetrominoNext.size() < NUM_PREVIEW_MINOS) {
		tetrominoNext.push(tetrominoGenerated.front());
		tetrominoGenerated.pop();
	}
}

void Game::spawn() {
	currentTetromino.reset();
	currentTetromino.mType = tetrominoNext.front();
	tetrominoNext.pop();
	switch (currentTetromino.mType) {
	case TETROMINO_I:
		writeToBoard(I_MINO);
		currentTetromino.mTetromino = I_MINO;
		break;
	case TETROMINO_O:
		writeToBoard(O_MINO);
		currentTetromino.mTetromino = O_MINO;
		break;
	case TETROMINO_T:
		writeToBoard(T_MINO);
		currentTetromino.mTetromino = T_MINO;
		break;
	case TETROMINO_S:
		writeToBoard(S_MINO);
		currentTetromino.mTetromino = S_MINO;
		break;
	case TETROMINO_Z:
		writeToBoard(Z_MINO);
		currentTetromino.mTetromino = Z_MINO;
		break;
	case TETROMINO_J:
		writeToBoard(J_MINO);
		currentTetromino.mTetromino = J_MINO;
		break;
	case TETROMINO_L:
		writeToBoard(L_MINO);
		currentTetromino.mTetromino = L_MINO;
		break;
	default:
		break;
	}
}

void Game::writeToBoard(vector<vector<int>> tetromino, bool clear) {
	int newRow, newCol;
	if (clear) {
		for (size_t row = 0; row < tetromino.size(); row++) {
			for (size_t col = 0; col < tetromino[0].size(); col++) {
				newRow = currentTetromino.row + row;
				newCol = currentTetromino.col + col;
				if (gameBoard[newRow][newCol] - tetromino[row][col] == 0) {
					gameBoard[newRow][newCol] = 0;
				}
			}
		}
	}
	else {
		for (size_t row = 0; row < tetromino.size(); row++) {
			for (size_t col = 0; col < tetromino[0].size(); col++) {
				newRow = currentTetromino.row + row;
				newCol = currentTetromino.col + col;
				if (tetromino[row][col] != 0) {
					gameBoard[newRow][newCol] = tetromino[row][col];
				}
			}
		}
	}
}

void Game::tick() {
	
}

void Game::move(int direction) {
	writeToBoard(currentTetromino.mTetromino, true);
	if (!willCollide(direction)) {
		switch (direction) {
		case DOWN:
			currentTetromino.row++;
			break;
		case LEFT:
			currentTetromino.col--;
			break;
		case RIGHT:
			currentTetromino.col++;
			break;
		default:
			break;
		}
		writeToBoard(currentTetromino.mTetromino);
	}
	else {
		writeToBoard(currentTetromino.mTetromino);
		if (direction == DOWN) { spawn(); }
	}
}

bool Game::willCollide(int direction) {
	int newRow, newCol;
	for (int row = 0; row < currentTetromino.mTetromino.size(); row++) {
		for (int col = 0; col < currentTetromino.mTetromino[0].size(); col++) {
			newRow = currentTetromino.row + row;
			newCol = currentTetromino.col + col;
			switch (direction) {
			case DOWN:
				newRow++;
				break;
			case RIGHT:
				newCol++;
				break;
			case LEFT:
				newCol--;
				break;
			}
			if (currentTetromino.mTetromino[row][col] != 0) {
				if (newRow >= NUM_ROWS || newCol >= NUM_COLS || newCol < 0) { return true; } // Out of bounds
				if (currentTetromino.mTetromino[row][col] - gameBoard[newRow][newCol] != currentTetromino.mTetromino[row][col]) { return true; }
			}
		}
	}
	return false;
}

void Game::updateScreen() {
	renderer.clear();
	renderer.update(gameBoard);
	renderer.present();
}

void Game::close() {
}

void Game::printBoard() {
	for (int i = 0; i < NUM_VISIBLE_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			cout << gameBoard[i][j];
		}
		cout << endl;
	}
}

Game::Tetromino::Tetromino() {
	reset();
}

void Game::Tetromino::reset() {
	row = TETROMINO_SPAWN_ROW;
	col = TETROMINO_SPAWN_COL;
	mType = 0;
	mTetromino.clear();
	mTetromino.shrink_to_fit();
}