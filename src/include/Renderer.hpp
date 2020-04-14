#pragma once
#include "Texture.hpp"
#include "TetrominoTypes.hpp"
#include "constants.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;
using namespace constants;

class Renderer
{
private:

	SDL_Window* sdlWindow = NULL;
	SDL_Renderer* sdlRenderer = NULL;

	Texture* backgroundTexture;
	Texture* tetrominoTexture;
	SDL_Rect tetrominoSpriteClips[TotalTypes];

	void setTetrominoSpriteClip(int tetrominoType, int x, int y, int w, int h);
	void Renderer::setTetrominoSpriteClip(int tetrominoType, int x, int y);

public:
	Renderer();
	~Renderer();

	bool initialize(string title, int width, int height);
	bool loadMedia();
	void clear();
	void update(int board[][NUM_COLS]);
	void present();
	void close();
};

