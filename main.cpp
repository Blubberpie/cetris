#include "Game.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	Game* game = new Game();
	if (game->initialize()) {
		game->run();
	}

	game->close();
	delete(game);
	game = NULL;
	return 0;
}