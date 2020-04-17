#pragma once
#include "Texture.hpp"
#include "TetrominoTypes.hpp"
#include "TetrominoShape.hpp"
#include "constants.hpp"
#include <queue>
#include <vector>

using namespace std;
using namespace constants;

class Renderer
{
private:

	SDL_Window* sdlWindow = NULL;
	SDL_Renderer* sdlRenderer = NULL;

	TTF_Font* font;
	TTF_Font* smallFont;
	Texture* backgroundTexture;
	Texture* tetrominoTexture;
	Texture* textTexture;
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
	void renderGhost(vector<vector<int>>& tetromino, int row, int col);
	void renderHoldBox(int holdType);
	void renderNextBox(queue<int> tetrominoNext);
	void renderText(string text, int x, int y, SDL_Color textColor, bool centerX = false, bool centerY = false, bool isSmall = false);
	void update(int board[][NUM_COLS]);
	void present();
	void close();
};

