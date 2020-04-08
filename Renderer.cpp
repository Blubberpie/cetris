#include "Renderer.hpp"
using namespace std;

Renderer::Renderer() {
}

Renderer::~Renderer() {
	close();
}

bool Renderer::initialize(string title, int width, int height) {
	bool success = true;
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL) { 
		success = false; 
		cout << "SDL could not create window! " << SDL_GetError();
	}
	else {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // Use hardware acceleration
		if (renderer == NULL) {
			success = false;
			cout << "SDL could not create renderer! " << SDL_GetError();
		}
		else {
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				success = false;
				cout << "SDL_image could not initialize! " << IMG_GetError();
			}
		}
	}
	return success;
}

bool Renderer::loadMedia() {
	bool success = true;
	backgroundTexture = loadTexture("images/background.png");
	if (backgroundTexture == NULL) {
		success = false;
		cout << "Couldn't load background texture!";
	}
	return success;
}

SDL_Texture* Renderer::loadTexture(string path) {
	SDL_Texture* loadedTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		cout << "Couldn't load file: " << path.c_str() << " IMG_Load error: " << IMG_GetError();
	}
	else {
		loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (loadedTexture == NULL) {
			cout << "Couldn't create texture from surface! " << SDL_GetError();
		}
		SDL_FreeSurface(loadedSurface);
	}
	return loadedTexture;
}

void Renderer::update() {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Renderer::close() {
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	backgroundTexture = NULL;
	renderer = NULL;
	window = NULL;

	IMG_Quit();
	SDL_Quit();
}