#include "Game.hpp"

Game::Game() {
	renderer = new Renderer();
}

Game::~Game() {
	close();
}

bool Game::initialize() {
	bool success = true;
	if (!renderer->initialize(GAME_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT)) { success = false; } 
	else if (!renderer->loadMedia()) { success = false; }
	return success; 
}

void Game::run() {
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		renderer->update();
	}
}

void Game::close() {
	renderer->close();
	delete(renderer);
	renderer = NULL;
}