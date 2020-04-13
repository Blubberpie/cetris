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
	srand(time(nullptr));
	bool quit = false;
	SDL_Event event;
	checkAndGenerateTetrominoSet();
	checkAndRefillTetrominoQueue();
	spawn();
	printBoard();
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			else if(event.type == SDL_KEYDOWN){
				if (event.key.keysym.sym == SDLK_DOWN) {
					tick();
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
		for (int row = 0; row < tetromino.size(); row++) {
			for (int col = 0; col < tetromino[0].size(); col++) {
				newRow = currentTetromino.row + row;
				newCol = currentTetromino.col + col;
				if (gameBoard[newRow][newCol] - tetromino[row][col] == 0) {
					gameBoard[newRow][newCol] = 0;
				}
			}
		}
	}
	else {
		for (int row = 0; row < tetromino.size(); row++) {
			for (int col = 0; col < tetromino[0].size(); col++) {
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
	writeToBoard(currentTetromino.mTetromino, true);
	if (!willOverlap()) {
		currentTetromino.row++;
		writeToBoard(currentTetromino.mTetromino);
	}
	else {
		writeToBoard(currentTetromino.mTetromino);
		spawn();
	}
}

bool Game::willOverlap() {
	int newRow, newCol;
	for (int row = currentTetromino.mTetromino.size() - 1; row >= 0; row--) {
		for (int col = currentTetromino.mTetromino[0].size() - 1; col >= 0; col--) {
			newRow = currentTetromino.row + row + 1;
			newCol = currentTetromino.col + col;
			if (currentTetromino.mTetromino[row][col] != 0) {
				if (newRow >= NUM_ROWS) { 
					cout << "out of range!\n";
					return true; 
				}
				if (currentTetromino.mTetromino[row][col] - gameBoard[newRow][newCol] != currentTetromino.mTetromino[row][col]) {
					cout << newRow << " " << newCol << endl;
					return true;
				}
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