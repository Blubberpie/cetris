#pragma once
#include "Texture.hpp"
#include "TetrominoTypes.hpp"
#include "TetrominoShape.hpp"
#include "constants.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <queue>
#include <vector>

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
	void renderTetromino(vector<vector<int>>& tetromino, int x, int y);

public:
	Renderer();
	~Renderer();

	bool initialize(string title, int width, int height);
	bool loadMedia();
	void clear();
	void renderBackground();
	void renderHoldBox(int holdType);
	void renderNextBox(queue<int> tetrominoNext);
	void update(int board[][NUM_COLS]);
	void present();
	void close();
};

