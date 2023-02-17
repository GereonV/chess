#ifndef _GFX_RECTANGLE_HPP_
#define _GFX_RECTANGLE_HPP_

#include "quads.hpp"
#include "opengl/shaders.hpp"

namespace gfx {

	class rectangle_renderer : public quad_renderer<rectangle_renderer> {
	friend quad_renderer;
	public:
		rectangle_renderer(gl::shader const & vert) noexcept
		: quad_renderer{vert, RECTANGLE_FRAG} {}

		void set_color(float r, float g, float b, float a = 1) const noexcept {
			gl::set_uniform_4_floats(1, r, g, b, a);
		}

	};

}

#endif // _GFX_RECTANGLE_HPP_
