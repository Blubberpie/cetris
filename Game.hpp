#pragma once
#include "Renderer.hpp"
#include <iostream>
using namespace std;

class Game
{
private:
	const string GAME_TITLE = "Cetris";
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	const int ROW_COUNT = 40;
	const int ROW_VISIBLE_COUNT = 20;
	const int COL_COUNT = 10;

	Renderer* renderer = nullptr;

public:
	Game();
	~Game();

	bool initialize();
	void run();
	void close();
};