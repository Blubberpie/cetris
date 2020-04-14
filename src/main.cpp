#include "include/Game.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	unique_ptr<Game> game(new Game());
	if (game->initialize()) {
		game->run();
	}

	return 0;
}