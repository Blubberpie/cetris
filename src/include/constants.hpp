#pragma once
#include <string>

namespace constants {
	inline const std::string GAME_TITLE = "Cetris";

	inline constexpr int SCREEN_RATIO_WIDTH = 16;
	inline constexpr int SCREEN_RATIO_HEIGHT = 9;
	inline constexpr int SCREEN_WIDTH = 1280;
	inline constexpr int SCREEN_HEIGHT = SCREEN_WIDTH * SCREEN_RATIO_HEIGHT / SCREEN_RATIO_WIDTH;
	inline constexpr int BOARD_HEIGHT = SCREEN_HEIGHT / 3 * 2; // 1.5x the height
	inline constexpr int BOARD_WIDTH = BOARD_HEIGHT / 2;

	inline constexpr int TILE_SPRITE_SIZE = 24; // Size of provided texture image
	inline constexpr int NUM_ROWS = 21;
	inline constexpr int NUM_VISIBLE_ROWS = 20;
	inline constexpr int NUM_COLS = 10;
	inline constexpr int TILE_RENDER_SIZE = BOARD_HEIGHT / NUM_VISIBLE_ROWS; // Size of tile to render
	inline constexpr int BOARD_START_X = (SCREEN_WIDTH / 2) - (TILE_RENDER_SIZE * NUM_COLS/ 2);
	inline constexpr int BOARD_START_Y = (SCREEN_HEIGHT / 2) - (TILE_RENDER_SIZE * NUM_VISIBLE_ROWS / 2);
	inline constexpr int HOLD_START_X = BOARD_START_X - (5 * TILE_RENDER_SIZE);
	inline constexpr int HOLD_START_Y = BOARD_START_Y + TILE_RENDER_SIZE;
	inline constexpr int QUEUE_START_X = BOARD_START_X + (TILE_RENDER_SIZE * (1 + NUM_COLS));
	inline constexpr int QUEUE_START_Y = HOLD_START_Y;
	inline constexpr int NUM_TETROMINO_QUEUE = 14;
	inline constexpr int NUM_PREVIEW_MINOS = 5;
	inline constexpr int NUM_TETROMINO = 7;
	inline constexpr int TETROMINO_SPAWN_ROW = NUM_ROWS - NUM_VISIBLE_ROWS - 1; // One row above the first visible row
	inline constexpr int TETROMINO_SPAWN_COL = (NUM_COLS / 2) - 2; // Two tiles left of the rough board center

	inline constexpr int DROP_SPEED = 900; // 1 tile per ? ms
}