#include "Renderer.hpp"

Renderer::Renderer() {
	backgroundTexture = new BackgroundTexture();
	tetrominoTexture = new TetrominoTexture();
}

Renderer::~Renderer() {
	close();
}

bool Renderer::initialize(string title, int width, int height) {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		success = false;
		cout << "SDL could not initialize! " << SDL_GetError();
	}
	else {
		sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (sdlWindow == NULL) {
			success = false;
			cout << "SDL could not create window! " << SDL_GetError();
		}
		else {
			// Use hardware acceleration
			sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
			if (sdlRenderer == NULL) {
				success = false;
				cout << "SDL could not create renderer! " << SDL_GetError();
			}
			else {
				SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					success = false;
					cout << "SDL_image could not initialize! " << IMG_GetError();
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
		cout << "Couldn't load background texture!";
	}
	if (!tetrominoTexture->loadFromFile(sdlRenderer, "images/tileset_clean.png")) {
		success = false;
		cout << "Couldn't load tetrominos texture!";
	}
	else {
		setTetrominoSpriteClip(TETROMINO_I, 0, 0);
		setTetrominoSpriteClip(TETROMINO_O, TILE_SPRITE_SIZE, 0);
		setTetrominoSpriteClip(TETROMINO_T, TILE_SPRITE_SIZE * 2, 0);
		setTetrominoSpriteClip(TETROMINO_S, TILE_SPRITE_SIZE * 3, 0);
		setTetrominoSpriteClip(TETROMINO_Z, TILE_SPRITE_SIZE * 4, 0);
		setTetrominoSpriteClip(TETROMINO_J, TILE_SPRITE_SIZE * 5, 0);
		setTetrominoSpriteClip(TETROMINO_L, TILE_SPRITE_SIZE * 6, 0);
	}

	return success;
}

void Renderer::setTetrominoSpriteClip(TetrominoTypes tetrominoType, int x, int y, int w, int h) {
	tetrominoSpriteClips[tetrominoType].x = x;
	tetrominoSpriteClips[tetrominoType].y = y;
	tetrominoSpriteClips[tetrominoType].w = w;
	tetrominoSpriteClips[tetrominoType].h = h;
}

void Renderer::setTetrominoSpriteClip(TetrominoTypes tetrominoType, int x, int y) {
	tetrominoSpriteClips[tetrominoType].x = x;
	tetrominoSpriteClips[tetrominoType].y = y;
	tetrominoSpriteClips[tetrominoType].w = TILE_SPRITE_SIZE;
	tetrominoSpriteClips[tetrominoType].h = TILE_SPRITE_SIZE;
}

void Renderer::clear() {
	SDL_RenderClear(sdlRenderer);
}

void Renderer::update() {
	backgroundTexture->render(sdlRenderer, 0, 0);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			tetrominoTexture->render(sdlRenderer, (i * TILE_RENDER_SIZE) + BOARD_START_X, (j * TILE_RENDER_SIZE) + BOARD_START_Y, &tetrominoSpriteClips[TETROMINO_I]);
		}
	}
}

void Renderer::present() {
	SDL_RenderPresent(sdlRenderer);
}

void Renderer::close() {
	delete(backgroundTexture);
	delete(tetrominoTexture);
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);	
	sdlRenderer = NULL;
	sdlWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}