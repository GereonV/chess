#ifndef CHESS_RENDERING_HPP
#define CHESS_RENDERING_HPP

#include <GFX/quads.hpp>
#include "shadersrc.hpp"
#include "color.hpp"
#include "structs.hpp"

struct render_settings {
	float scale_x, scale_y;
	float size{1.5f}; // size=1 -> half screen, size=2 -> fillscreen

	void scale_for(int width, int height) noexcept {
		if(width > height) {
			scale_x = static_cast<float>(height) / static_cast<float>(width);
			scale_y = 1;
		} else {
			scale_x = 1;
			scale_y = static_cast<float>(width) / static_cast<float>(height);
		}
	}

};

class board_renderer : public gfx::quad_renderer<board_renderer> {
friend quad_renderer;
public:
	board_renderer(gfx::gl::shader const & vert, board_settings const & bs) noexcept
	: quad_renderer{vert, BOARD_FRAG} {
		use();
		set_light_color(bs.light_color);
		set_dark_color(bs.dark_color);
		if(bs.flipped)
			set_flipped(true);
	}

	void set_light_color(color c) const noexcept {
		gfx::gl::set_uniform_3_floats_arr(1, 1, c);
	}

	void set_dark_color(color c) const noexcept {
		gfx::gl::set_uniform_3_floats_arr(2, 1, c);
	}

	void set_flipped(bool flipped) const noexcept {
		gfx::gl::set_uniform_int(3, flipped);
	}

};

// after board.use()
inline void render(render_settings const & settings) noexcept {
	gfx::matrix transform{};
	transform[0][0] = settings.size * settings.scale_x;
	transform[1][1] = settings.size * settings.scale_y;
	transform[2][2] = 1.f;
	transform[3][3] = 1.f;
	gfx::set_transformation(transform);
	gfx::draw_quad();
	return; // TODO remove for individual squares
	transform[0][0] /= 8.f;
	transform[1][1] /= 8.f;
	gfx::set_transformation(transform);

	// transform[3][0] = <translate_x>;
	// transform[3][1] = <translate_y>;
}

#endif // CHESS_RENDERING_HPP
