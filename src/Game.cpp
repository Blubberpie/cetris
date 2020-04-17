#include "include/Game.hpp"

Game::Game() {
	downKey.direction = DOWN;
	downKey.delay = DROP_DELAY;
	downKey.repeatRate = DROP_REPEAT_RATE;
	leftKey.direction = LEFT;
	leftKey.delay = rightKey.delay = DELAYED_AUTO_SHIFT;
	leftKey.repeatRate = rightKey.repeatRate = AUTO_REPEAT_RATE;
	rightKey.direction = RIGHT;
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
	keyboardStates = SDL_GetKeyboardState(NULL);
	checkAndGenerateTetrominoSet();
	checkAndRefillTetrominoQueue();
	spawn();
	currentMenu = MAIN_MENU;
	while (!quit) {
		switch (currentMenu) {
		case MAIN_MENU:
			runMenu();
			break;
		case GAME:
			runCetris();
			break;
		case SETTINGS:
			break;
		}
	}
}

void Game::runMenu() {
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				switch (event.key.keysym.sym) {
				case SDLK_SPACE:
					gameOver = false;
					currentMenu = GAME;
					return;
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
			}
		}
		updateMainMenuScreen();
	}
}

// The main game
void Game::runCetris() {
	while (!gameOver && !quit) {
		// Note: poll all events before checking key states
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					gameOver = true;
					currentMenu = MAIN_MENU;
					break;
				case SDLK_w:
					hardDrop();
					break;
				case SDLK_q:
					if (!alreadyHeld) hold();
					break;
				case SDLK_DOWN:
					rotate(LEFT);
					break;
				case SDLK_RIGHT:
					rotate(RIGHT);
					break;
				}
			}
		}

		if (keyboardStates[SDL_SCANCODE_S]) handleKeyPress(downKey);
		else downKey.alreadyPressed = false;

		if (keyboardStates[SDL_SCANCODE_A] && !rightKey.alreadyPressed) {
			handleKeyPress(leftKey);
		}
		else if (rightKey.alreadyPressed) {
			handleKeyPress(rightKey);
			leftKey.alreadyPressed = false;
		}
		else {
			leftKey.alreadyPressed = false;
		}

		if (keyboardStates[SDL_SCANCODE_D] && !leftKey.alreadyPressed) {
			handleKeyPress(rightKey);
		}
		else if (leftKey.alreadyPressed) {
			handleKeyPress(leftKey);
			rightKey.alreadyPressed = false;
		}
		else {
			rightKey.alreadyPressed = false;
		}

		if (gameOver) return;

		tick();

		checkAndRefillTetrominoQueue();
		checkAndGenerateTetrominoSet();
		updateMainGameScreen();
	}
}

void Game::resetGame() {
	memset(gameBoard, 0, sizeof(gameBoard));
	lastHighestRow = NUM_ROWS - 1;
	holdType = EMPTY_TILE;
	alreadyHeld = false;
	dropped = false;

	currentTick = 0;
	lastTick = 0;
	lastLockDelay = 0;

	currentTetromino.reset();

	checkAndGenerateTetrominoSet();
	checkAndRefillTetrominoQueue();
	spawn();
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
	currentTetromino.mTetromino = TetrominoShape::getTetrominoShape(currentTetromino.mType);
	currentTetromino.ghostRow = calculateDrop();
	if (willCollide(currentTetromino.mTetromino, currentTetromino.row, currentTetromino.col)) {
		gameOver = true;
		currentMenu = MAIN_MENU;
		resetGame();
		return;
	}
	writeToBoard(currentTetromino.mTetromino);
	resetDirections();
}

void Game::writeToBoard(vector<vector<int>> &tetromino, bool clear) {
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
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime - lastTick >= DROP_TICK_DELAY) {
		move(DOWN);
		lastTick = currentTime;
	}
}

void Game::move(int direction) {
	writeToBoard(currentTetromino.mTetromino, true);
	switch (direction) {
	case DOWN:
		if (!willCollide(currentTetromino.mTetromino, currentTetromino.row + 1, currentTetromino.col)) {
			currentTetromino.row++;
		}
		else {
			if (!dropped) {
				lastLockDelay = SDL_GetTicks();
				dropped = true;
			}
			else {
				Uint32 currentTime = SDL_GetTicks();
				if (currentTime - lastLockDelay >= LOCK_DELAY) {
					lockTetromino();
					dropped = false;
				}
			}
		}
		break;
	case LEFT:
		if (!willCollide(currentTetromino.mTetromino, currentTetromino.row, currentTetromino.col - 1)) {
			currentTetromino.col--;
			if (dropped) {
				lastLockDelay = SDL_GetTicks();
			}
		}
		break;
	case RIGHT:
		currentTetromino.ghostRow = calculateDrop();
		if (!willCollide(currentTetromino.mTetromino, currentTetromino.row, currentTetromino.col + 1)) {
			currentTetromino.col++;
			if (dropped) {
				lastLockDelay = SDL_GetTicks();
			}
		}
		break;
	}
	currentTetromino.ghostRow = calculateDrop();
	writeToBoard(currentTetromino.mTetromino);
}

void Game::handleKeyPress(KeyboardKey& key) {
	if (!key.alreadyPressed) { // first time pressing
		move(key.direction);
		key.alreadyPressed = true;
		key.lastDelayTime = SDL_GetTicks();
	}
	else {
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - key.lastDelayTime >= key.delay) {
			if (currentTime - key.lastRepeatTime >= key.repeatRate) {
				move(key.direction);
				key.lastRepeatTime = currentTime;
			}
		}
	}
}

int Game::calculateDrop() {
	writeToBoard(currentTetromino.mTetromino, true);
	int newRow = currentTetromino.row + 1;
	while (!willCollide(currentTetromino.mTetromino, newRow, currentTetromino.col)) {
		newRow++;
	}
	return newRow - 1;
}

void Game::hardDrop() {
	writeToBoard(currentTetromino.mTetromino, true);
	currentTetromino.row = currentTetromino.ghostRow;
	if (!(currentTetromino.row < 0)) {
		lockTetromino();
		dropped = false;
	}
	else {
		gameOver = true;
		currentMenu = MAIN_MENU;
		resetGame();
	}
}

void Game::lockTetromino() {
	writeToBoard(currentTetromino.mTetromino);
	if (currentTetromino.row < lastHighestRow) { lastHighestRow = currentTetromino.row; }
	clearLines();
	spawn();
	alreadyHeld = false;
}

void Game::rotate(int direction) {
	writeToBoard(currentTetromino.mTetromino, true);
	if (currentTetromino.mTetromino.size() == currentTetromino.mTetromino[0].size()) {
		vector<vector<int>> copyTetromino = currentTetromino.mTetromino;
		switch (direction) {
		case LEFT:
			transposeLeft(copyTetromino);
			break;
		case RIGHT:
			transposeRight(copyTetromino);
			break;
		}
		reverseColumns(copyTetromino);
		if (passedKickTest(copyTetromino, direction)) {
			switch (direction) {
			case LEFT:
				if (currentTetromino.orientation - 1 < 0) {
					currentTetromino.orientation = LEFT;
				}
				else { currentTetromino.orientation--; 
				}
				break;
			case RIGHT:
				if (currentTetromino.orientation + 1 >= numDirections) {
					currentTetromino.orientation = 0;
				}
				else { currentTetromino.orientation++; }
				break;
			}
			if (dropped) {
				lastLockDelay = SDL_GetTicks();
			}
			currentTetromino.mTetromino = copyTetromino;
		}
	}
	currentTetromino.ghostRow = calculateDrop();
	writeToBoard(currentTetromino.mTetromino);
}

bool Game::passedKickTest(vector<vector<int>>& tetromino, int direction) {
	// Test 1
	if (!willCollide(tetromino, currentTetromino.row, currentTetromino.col)) {
		return true; 
	}
	else {
		if (currentTetromino.mType == TETROMINO_I) {
			switch (currentTetromino.orientation) {
			case UP:
				if (direction == RIGHT) { return(performKickTests(tetromino, KICK_TABLE_I_UP_RIGHT)); }
				else if (direction == LEFT) { return(performKickTests(tetromino, KICK_TABLE_I_RIGHT_DOWN)); }
			case RIGHT:
				if (direction == RIGHT) { return(performKickTests(tetromino, KICK_TABLE_I_RIGHT_DOWN)); }
				else if (direction == LEFT) { return(performKickTests(tetromino, KICK_TABLE_I_DOWN_LEFT)); }
			case DOWN:
				if (direction == RIGHT) { return(performKickTests(tetromino, KICK_TABLE_I_DOWN_LEFT)); }
				else if (direction == LEFT) { return(performKickTests(tetromino, KICK_TABLE_I_LEFT_UP)); }
				break;
			case LEFT:
				if (direction == RIGHT) { return(performKickTests(tetromino, KICK_TABLE_I_LEFT_UP)); }
				else if (direction == LEFT) { return(performKickTests(tetromino, KICK_TABLE_I_UP_RIGHT)); }
				break;
			}
		}
		else {
			switch (currentTetromino.orientation) {
			case UP:
				if (direction == RIGHT) { return(performKickTests(tetromino, KICK_TABLE_UP_RIGHT)); }
				else if (direction == LEFT) { return(performKickTests(tetromino, KICK_TABLE_DOWN_LEFT)); }
			case RIGHT:
				return(performKickTests(tetromino, KICK_TABLE_RIGHT_DOWN));
			case DOWN:
				if (direction == RIGHT) { return(performKickTests(tetromino, KICK_TABLE_DOWN_LEFT)); }
				else if (direction == LEFT) { return(performKickTests(tetromino, KICK_TABLE_UP_RIGHT)); }
				break;
			case LEFT:
				return(performKickTests(tetromino, KICK_TABLE_LEFT_UP));
				break;
			}
		}
	}
	return false;
}

bool Game::performKickTests(vector<vector<int>>& tetromino, int kickTable[4][2]) {
	// Tests 2 - 5
	for (int i = 0; i < 4; i++) {
		if (!willCollide(tetromino, currentTetromino.row + kickTable[i][0], currentTetromino.col + kickTable[i][1])) {
			currentTetromino.row += kickTable[i][0];
			currentTetromino.col += kickTable[i][1];
			return true;
		}
	}
	return false;
}

bool Game::willCollide(vector<vector<int>>& tetromino, int startRow, int startCol) {
	int newRow, newCol;
	for (size_t row = 0; row < tetromino.size(); row++) {
		for (size_t col = 0; col < tetromino[0].size(); col++) {
			newRow = startRow + row;
			newCol = startCol + col;
			if (tetromino[row][col] != 0) {
				if (newRow >= NUM_ROWS || newCol >= NUM_COLS || newCol < 0 || newRow < 0) { return true; } // Out of bounds
				if (tetromino[row][col] - gameBoard[newRow][newCol] != tetromino[row][col]) { return true; }
			}
		}
	}
	return false;
}

void Game::transposeLeft(vector<vector<int>>& tetromino) {
	for (size_t row = 0; row < tetromino.size(); row++) {
		for (size_t col = row; col < tetromino[0].size(); col++) {
			swap(tetromino[row][col], tetromino[col][row]);
		}
	}
}
void Game::transposeRight(vector<vector<int>>& tetromino) {
	for (size_t row = 0, bRow = tetromino.size() - 1; row < tetromino.size() && bRow >= 0; row++, bRow--) {
		for (size_t col = bRow, bCol = row; col >= 0 && bCol < tetromino[0].size(); col--, bCol++) {
			swap(tetromino[row][col], tetromino[bCol][bRow]);	
		}
	}
}

void Game::reverseColumns(vector<vector<int>>& tetromino) {
	for (size_t col = 0; col < tetromino[0].size(); col++) {
		for (size_t i = 0, j = tetromino[0].size() - 1; i < j; i++, j--) {
			swap(tetromino[i][col], tetromino[j][col]);
		}
	}
}

void Game::clearLines() { // TODO find that one glitch
	int totalLinesCleared = 0;
	int lastKnownRow = currentTetromino.row;
	// Only search a maximum of 4 lines, from the current tetromino's beginning row
	for (int row = lastKnownRow; row < lastKnownRow + 4 && row < NUM_ROWS; row++) {
		bool fullLine = true;
		for (int col = 0; col < NUM_COLS; col++) {
			if (gameBoard[row][col] == EMPTY_TILE) {
				fullLine = false;
				break;
			}
		}
		if (fullLine) {
			for (int col = 0; col < NUM_COLS; col++) {
				gameBoard[row][col] = 0;
			}
			cascade(row);
		}
	}
}

void Game::cascade(int endRow) {
	for (int row = endRow; row >= lastHighestRow; row--) {
		for (int col = 0; col < NUM_COLS; col++) {
			if (row - 1 >= 0) {
				gameBoard[row][col] = gameBoard[row - 1][col];
				gameBoard[row - 1][col] = EMPTY_TILE;
			}
		}
	}
}

void Game::hold() {
	alreadyHeld = true;
	writeToBoard(currentTetromino.mTetromino, true);
	if (holdType == EMPTY_TILE) {
		holdType = currentTetromino.mType;
		spawn();
	}
	else {
		int tmp = holdType;
		holdType = currentTetromino.mType;
		currentTetromino.reset();
		currentTetromino.mType = tmp;
		currentTetromino.mTetromino = TetrominoShape::getTetrominoShape(tmp);
		currentTetromino.ghostRow = calculateDrop();
		writeToBoard(currentTetromino.mTetromino);
		resetDirections();
	}
}

void Game::resetDirections() {
	leftKey.alreadyPressed = false;
	rightKey.alreadyPressed = false;
}

void Game::updateMainGameScreen() {
	renderer.clear();
	renderer.renderBackground();
	SDL_Color textColor = { 213, 213, 213 };
	renderer.renderText("Hold", HOLD_START_X, HOLD_START_Y - FONT_SIZE - 16, textColor);
	renderer.renderText("Next", QUEUE_START_X, QUEUE_START_Y - FONT_SIZE - 16, textColor);
	renderer.update(gameBoard);
	if (currentTetromino.ghostRow > currentTetromino.row) {
		renderer.renderGhost(currentTetromino.mTetromino, currentTetromino.ghostRow, currentTetromino.col);
	}
	renderer.renderHoldBox(holdType);
	renderer.renderNextBox(tetrominoNext);
	renderer.present();
}

void Game::updateMainMenuScreen() {
	renderer.clear();
	renderer.renderBackground();
	SDL_Color textColor;
	if (gameOver) {
		textColor = { 220, 16, 72 };
		renderer.renderText("GAME OVER", 0, 22, textColor, true);
	}
	textColor = { 50, 200, 220 };
	renderer.renderText("Cetris", 0, (SCREEN_HEIGHT / 2) - (FONT_SIZE * 2), textColor, true);
	textColor = { 213, 213, 213 };
	renderer.renderText("Press \"space\" to play!", 0, 0, textColor, true, true);
	renderer.present();
}

void Game::close() {
}

Game::Tetromino::Tetromino() {
	reset();
}

void Game::Tetromino::reset() {
	row = TETROMINO_SPAWN_ROW;
	col = TETROMINO_SPAWN_COL;
	mType = 0;
	orientation = UP;
	mTetromino.clear();
	mTetromino.shrink_to_fit();
}

// Print 2D array for debugging
void Game::printBoard() {
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			cout << gameBoard[i][j];
		}
		cout << endl;
	}
	cout << endl;
}