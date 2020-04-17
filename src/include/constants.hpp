#pragma once
#include <string>

namespace constants {
	inline const std::string GAME_TITLE = "Cetris";
	inline const std::string FONT_PATH = "fonts/gidole_regular.ttf";

	// Screen 

	inline constexpr int SCREEN_RATIO_WIDTH = 16;
	inline constexpr int SCREEN_RATIO_HEIGHT = 9;
	inline constexpr int SCREEN_WIDTH = 1280;
	inline constexpr int SCREEN_HEIGHT = SCREEN_WIDTH * SCREEN_RATIO_HEIGHT / SCREEN_RATIO_WIDTH;
	inline constexpr int FONT_SIZE = 36;
	inline constexpr int SMALL_FONT_SIZE = 24;

	// Playfield 

	inline constexpr int BOARD_HEIGHT = SCREEN_HEIGHT / 3 * 2; // Height of the playfield
	inline constexpr int BOARD_WIDTH = BOARD_HEIGHT / 2; // Width of the playfield
	inline constexpr int TILE_SPRITE_SIZE = 24; // Size of provided texture image
	inline constexpr int NUM_ROWS = 22;
	inline constexpr int NUM_VISIBLE_ROWS = 20;
	inline constexpr int NUM_COLS = 10;
	inline constexpr int TILE_RENDER_SIZE = BOARD_HEIGHT / NUM_VISIBLE_ROWS; // Size of tile to render
	inline constexpr int BOARD_START_X = (SCREEN_WIDTH / 2) - (TILE_RENDER_SIZE * NUM_COLS/ 2);
	inline constexpr int BOARD_START_Y = (SCREEN_HEIGHT / 2) - (TILE_RENDER_SIZE * NUM_VISIBLE_ROWS / 2);
	inline constexpr int HOLD_START_X = BOARD_START_X - (5 * TILE_RENDER_SIZE);
	inline constexpr int HOLD_START_Y = BOARD_START_Y;
	inline constexpr int QUEUE_START_X = BOARD_START_X + (TILE_RENDER_SIZE * (1 + NUM_COLS));
	inline constexpr int QUEUE_START_Y = HOLD_START_Y;

	// Gameplay

	inline constexpr int NUM_TETROMINO_QUEUE = 14;
	inline constexpr int NUM_PREVIEW_MINOS = 5;
	inline constexpr int NUM_TETROMINO = 7;
	inline constexpr int TETROMINO_SPAWN_ROW = NUM_ROWS - NUM_VISIBLE_ROWS - 1; // One row above the first visible row
	inline constexpr int TETROMINO_SPAWN_COL = (NUM_COLS / 2) - 2; // Two tiles left of the rough board center

	inline constexpr unsigned int DROP_TICK_DELAY = 700; // 1 tile per ? ms
	inline constexpr unsigned int DROP_DELAY = 60; // How long a key should be held before repetition is triggered
	inline constexpr unsigned int DROP_REPEAT_RATE = 17; // How fast to drop when key is held
	inline constexpr unsigned int DELAYED_AUTO_SHIFT = 133; // How long a key should be held before repetition is triggered
	inline constexpr unsigned int AUTO_REPEAT_RATE = 17; // How fast a block moves left/right
	inline constexpr unsigned int LOCK_DELAY = 500;

	inline constexpr int WRAP_LENGTH = HOLD_START_X - 32;
}