#pragma once
#include "constants.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;
using namespace constants;

class Texture {
private:

	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

	void free();

public:

	Texture();
	~Texture();

	virtual void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL);
	bool loadFromFile(SDL_Renderer* renderer, string path);
	int getWidth();
	int getHeight();
	SDL_Texture* getTexture();
};

class BackgroundTexture : public Texture {
public:
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL);
};

class TetrominoTexture : public Texture {
public:
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip);
};

