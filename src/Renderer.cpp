#include "include/Renderer.hpp"

Renderer::Renderer() {
	backgroundTexture = new BackgroundTexture();
	tetrominoTexture = new TetrominoTexture();
	textTexture = new Texture();
}

Renderer::~Renderer() {
	close();
}

bool Renderer::initialize(string title, int width, int height) {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		success = false;
		cout << "SDL could not initialize! " << SDL_GetError() << endl;
	}
	else {
		sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (sdlWindow == NULL) {
			success = false;
			cout << "SDL could not create window! " << SDL_GetError() << endl;
		}
		else {
			// Use hardware acceleration
			sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
			if (sdlRenderer == NULL) {
				success = false;
				cout << "SDL could not create renderer! " << SDL_GetError() << endl;
			}
			else {
				SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					success = false;
					cout << "SDL_image could not initialize! " << IMG_GetError() << endl;
				}

				if (TTF_Init() == -1) {
					success = false;
					cout << "SDL_ttf could not initialize! " << TTF_GetError() << endl;
				}
			}
		}
	}
	return success;
}

bool Renderer::loadMedia() {
	bool success = true;
	if (!backgroundTexture->loadFromFile(sdlRenderer, "images/background.png")) {
		success = false;
		cout << "Couldn't load background texture!\n";
	}
	if (!tetrominoTexture->loadFromFile(sdlRenderer, "images/tileset_clean.png")) {
		success = false;
		cout << "Couldn't load tetrominos texture!\n";
	}
	else {
		tetrominoTexture->setBlendMode(SDL_BLENDMODE_BLEND);
		for (int i = 0; i < TotalTypes; i++) {
			setTetrominoSpriteClip(i, TILE_SPRITE_SIZE * i, 0);
		}
	}
	font = TTF_OpenFont("font/gidole_regular.ttf", FONT_SIZE);
	smallFont = TTF_OpenFont("font/gidole_regular.ttf", SMALL_FONT_SIZE);
	if (font == NULL || smallFont == NULL) {
		cout << "Failed to load font! " << TTF_GetError() << endl;
	}
	return success;
}

void Renderer::setTetrominoSpriteClip(int tetrominoType, int x, int y, int w, int h) {
	tetrominoSpriteClips[tetrominoType].x = x;
	tetrominoSpriteClips[tetrominoType].y = y;
	tetrominoSpriteClips[tetrominoType].w = w;
	tetrominoSpriteClips[tetrominoType].h = h;
}

void  Renderer::setTetrominoSpriteClip(int tetrominoType, int x, int y) {
	tetrominoSpriteClips[tetrominoType].x = x;
	tetrominoSpriteClips[tetrominoType].y = y;
	tetrominoSpriteClips[tetrominoType].w = TILE_SPRITE_SIZE;
	tetrominoSpriteClips[tetrominoType].h = TILE_SPRITE_SIZE;
}

void Renderer::clear() {
	SDL_RenderClear(sdlRenderer);
}

void Renderer::renderBackground() {
	backgroundTexture->render(sdlRenderer, 0, 0);
}

void Renderer::renderTetromino(vector<vector<int>>& tetromino, int x, int y) {
	if (!tetromino.empty()) {
		for (size_t row = 0; row < tetromino.size(); row++) {
			for (size_t col = 0; col < tetromino[0].size(); col++) {
				int currentTile = tetromino[row][col];
				if (currentTile != EMPTY_TILE && ((row * TILE_RENDER_SIZE) + y) >= BOARD_START_Y) {
					tetrominoTexture->render(sdlRenderer, (col * TILE_RENDER_SIZE) + x, (row * TILE_RENDER_SIZE) + y, &tetrominoSpriteClips[currentTile]);
				}
			}
		}
	}
}

void Renderer::renderGhost(vector<vector<int>>& tetromino, int row, int col) {
	tetrominoTexture->setAlpha(127);
	renderTetromino(tetromino, (col * TILE_RENDER_SIZE) + BOARD_START_X, (row * TILE_RENDER_SIZE) + BOARD_START_Y - ((NUM_ROWS - NUM_VISIBLE_ROWS) * TILE_RENDER_SIZE));
	tetrominoTexture->setAlpha(255);
}

void Renderer::renderHoldBox(int holdType) {
	vector<vector<int>> holdTetromino = TetrominoShape::getTetrominoShape(holdType);
	renderTetromino(holdTetromino, HOLD_START_X, HOLD_START_Y);
}

void Renderer::renderNextBox(queue<int> tetrominoNext) {
	int startY = QUEUE_START_Y;
	vector<vector<int>> nextTetromino;
	while (!tetrominoNext.empty()) {
		nextTetromino = TetrominoShape::getTetrominoShape(tetrominoNext.front());
		tetrominoNext.pop();
		renderTetromino(nextTetromino, QUEUE_START_X, startY);
		startY += (4 * TILE_RENDER_SIZE);
	}
}

void Renderer::renderText(string text, int x, int y, SDL_Color textColor, bool centerX, bool centerY, bool isSmall) {
	int newX = x;
	int newY = y;
	if (isSmall) {
		textTexture->loadFromRenderedText(sdlRenderer, smallFont, text, textColor);
	}
	else {
		textTexture->loadFromRenderedText(sdlRenderer, font, text, textColor);
	}
	if (centerX) newX = (SCREEN_WIDTH / 2) - (textTexture->getWidth() / 2);
	if (centerY) newY = (SCREEN_HEIGHT / 2) - (textTexture->getHeight() / 2);
	textTexture->render(sdlRenderer, newX, newY, NULL);
}

void Renderer::update(int board[][NUM_COLS]) {
	int startY = BOARD_START_Y - ((NUM_ROWS - NUM_VISIBLE_ROWS) * TILE_RENDER_SIZE);
	for (int row = NUM_ROWS - NUM_VISIBLE_ROWS; row < NUM_ROWS; row++) {
		for (int col = 0; col < NUM_COLS; col++) {
			int currentTile = board[row][col];
			tetrominoTexture->render(sdlRenderer, (col * TILE_RENDER_SIZE) + BOARD_START_X, (row * TILE_RENDER_SIZE) + startY, &tetrominoSpriteClips[currentTile]);
		}
	}
}

void Renderer::present() {
	SDL_RenderPresent(sdlRenderer);
}

void Renderer::close() {
	delete(backgroundTexture);
	delete(tetrominoTexture);
	delete(textTexture);
	TTF_CloseFont(font);
	font = NULL;
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);	
	sdlRenderer = NULL;
	sdlWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}