#pragma once
#include <string>

namespace constants {
	inline const std::string GAME_TITLE = "Cetris";

	// Screen
	inline constexpr int SCREEN_RATIO_WIDTH = 16;
	inline constexpr int SCREEN_RATIO_HEIGHT = 9;
	inline constexpr int SCREEN_WIDTH = 1280;
	inline constexpr int SCREEN_HEIGHT = SCREEN_WIDTH * SCREEN_RATIO_HEIGHT / SCREEN_RATIO_WIDTH;
	inline constexpr float SCREEN_HEIGHT_TO_BOARD_HEIGHT_RATIO = 1.5;

	// Game board
	inline constexpr int TILE_SPRITE_SIZE = 24; // Size of provided texture image
	inline constexpr int ROW_COUNT = 40;
	inline constexpr int ROW_VISIBLE_COUNT = 20;
	inline constexpr int COL_COUNT = 10;
	inline constexpr int TILE_RENDER_SIZE = SCREEN_HEIGHT / SCREEN_HEIGHT_TO_BOARD_HEIGHT_RATIO / ROW_VISIBLE_COUNT; // Size of tile to render
	inline constexpr int BOARD_START_X = (SCREEN_WIDTH / 2) - (TILE_RENDER_SIZE * COL_COUNT/ 2);
	inline constexpr int BOARD_START_Y = (SCREEN_HEIGHT / 2) - (TILE_RENDER_SIZE * ROW_VISIBLE_COUNT / 2);
}