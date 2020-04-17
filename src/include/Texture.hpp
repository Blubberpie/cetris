#pragma once
#include "constants.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
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
	bool loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, string text, SDL_Color color);

	SDL_Texture* getTexture();
	int getWidth();
	int getHeight();
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
};

class BackgroundTexture : public Texture {
public:
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL);
};

class TetrominoTexture : public Texture {
public:
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip);
};

