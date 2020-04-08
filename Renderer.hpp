#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
using namespace std;

class Renderer
{
private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* backgroundTexture = NULL;

public:
	Renderer();
	~Renderer();

	bool initialize(string title, int width, int height);
	bool loadMedia();
	void update();
	void close();
	SDL_Texture* loadTexture(string path);
};

