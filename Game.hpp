#pragma once
#include "Renderer.hpp"
#include "constants.hpp"
#include <iostream>

using namespace std;
using namespace constants;

class Game
{
private:
	Renderer* renderer = nullptr;

public:
	Game();
	~Game();

	bool initialize();
	void run();
	void close();
};