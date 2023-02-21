#ifndef CHESS_STRUCTS_HPP
#define CHESS_STRUCTS_HPP

// config and session
struct board_config {
	color light_color{.6f, .6f, .7f};  // #9999B3
	color dark_color{.35f, .35f, .5f}; // #595980
};

// session view options
struct board_settings : board_config {
	bool flipped{false};
};

// config file
struct config {
	board_config board;
};

#endif // CHESS_STRUCTS_HPP
