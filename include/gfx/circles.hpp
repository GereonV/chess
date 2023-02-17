#ifndef _GFX_CIRCLES_HPP_
#define _GFX_CIRCLES_HPP_

#include "quads.hpp"
#include "opengl/shaders.hpp"

namespace gfx {

	class circle_renderer : public quad_renderer<circle_renderer> {
	friend quad_renderer;
	public:
		circle_renderer(gl::shader const & vert) noexcept
		: quad_renderer{vert, CIRCLE_FRAG, true} {}

		void set_color(float r, float g, float b, float a = 1) const noexcept {
			gl::set_uniform_4_floats(1, r, g, b, a);
		}

	};

}

#endif // _GFX_CIRCLES_HPP_
