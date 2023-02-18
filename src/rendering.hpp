#ifndef GFX_RECTANGLE_HPP
#define GFX_RECTANGLE_HPP

#include <GFX/quads.hpp>
#include "shadersrc.hpp"

class board_renderer : public gfx::quad_renderer<board_renderer> {
friend quad_renderer;
public:
	board_renderer(gfx::gl::shader const & vert) noexcept
	: quad_renderer{vert, BOARD_FRAG} {}

	void set_white_color(float r, float g, float b) const noexcept {
		gfx::gl::set_uniform_3_floats(1, r, g, b);
	}

	void set_black_color(float r, float g, float b) const noexcept {
		gfx::gl::set_uniform_3_floats(2, r, g, b);
	}

	void view_from_white() const noexcept {
		gfx::gl::set_uniform_int(3, 0);
	}

	void view_from_black() const noexcept {
		gfx::gl::set_uniform_int(3, 1);
	}

};

#endif // GFX_RECTANGLE_HPP
