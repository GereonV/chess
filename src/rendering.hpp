#ifndef GFX_RECTANGLE_HPP
#define GFX_RECTANGLE_HPP

#include <GFX/quads.hpp>
#include "shadersrc.hpp"

struct color {
private:
	using data = float[3];
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

struct board_view {
	color light_color;
	color dark_color;
	bool flipped{false};
};

class board_renderer : public gfx::quad_renderer<board_renderer> {
friend quad_renderer;
public:
	board_renderer(gfx::gl::shader const & vert) noexcept
	: quad_renderer{vert, BOARD_FRAG} {}

	void set_white_color(color c) const noexcept {
		gfx::gl::set_uniform_3_floats_arr(1, 1, c);
	}

	void set_black_color(color c) const noexcept {
		gfx::gl::set_uniform_3_floats_arr(2, 1, c);
	}

	void set_flipped(bool flipped) const noexcept {
		gfx::gl::set_uniform_int(3, flipped);
	}
};

#endif // GFX_RECTANGLE_HPP
