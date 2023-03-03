#ifndef CHESS_STRUCTS_HPP
#define CHESS_STRUCTS_HPP

#include "color.hpp"

// config and session
struct board_config {
	color light_color{.6f, .6f, .7f};  // #9999B3
	color dark_color{.35f, .35f, .5f}; // #595980
};

// session view options
struct board_settings : board_config {
	bool flipped{false};
};

struct color {
private:
	using      data = float[3];
	using       ref = data       &;
	using const_ref = data const &;
public:
	color() = default;
	constexpr color(float r, float g, float b) : _rgb{r, g, b} {}
	constexpr void r(float r) noexcept { _rgb[0] = r; }
	constexpr void g(float g) noexcept { _rgb[1] = g; }
	constexpr void b(float b) noexcept { _rgb[2] = b; }
	constexpr void set(float r, float g, float b) noexcept {
		_rgb[0] = r;
		_rgb[1] = g;
		_rgb[2] = b;
	}

	constexpr float r() const noexcept { return _rgb[0]; }
	constexpr float g() const noexcept { return _rgb[1]; }
	constexpr float b() const noexcept { return _rgb[2]; }
	constexpr       ref rgb()       noexcept { return _rgb; }
	constexpr const_ref rgb() const noexcept { return _rgb; }
	constexpr operator       ref()       noexcept { return _rgb; }
	constexpr operator const_ref() const noexcept { return _rgb; }
private:
	data _rgb;
};

#endif // CHESS_STRUCTS_HPP
