#ifndef _GFX_GL_DRAWING_HPP_
#define _GFX_GL_DRAWING_HPP_

#include "loader.hpp"

namespace gfx::gl {

	class clearer {
	public:
		constexpr clearer(bool color, bool depth) noexcept : mask_{} {
			if(color)
				mask_ |= GL_COLOR_BUFFER_BIT;
			if(depth)
				mask_ |= GL_DEPTH_BUFFER_BIT;
			// stencil missing (see below)
		}

		void operator()() const noexcept { glClear(mask_); }
	private:
		GLbitfield mask_;
	};

	enum class polygon_mode : GLenum {
		point = GL_POINT,
		line  = GL_LINE,
		fill  = GL_FILL
	};

	inline void set_polygon_mode(polygon_mode mode) noexcept {
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(mode));
	}

	enum class blending_factor : GLenum {
		zero = GL_ZERO,
		one = GL_ONE,
		src_alpha = GL_SRC_ALPHA,
		inv_src_alpha = GL_ONE_MINUS_SRC_ALPHA,
		dst_alpha = GL_DST_ALPHA,
		inv_dst_alpha = GL_ONE_MINUS_DST_ALPHA,
		// GL_XXX_COLOR & GL_SRC_ALPHA_SATURATE missing
		// GL...CONSTANT... & glBlendColor() missing
	};

	inline void set_blending(blending_factor src, blending_factor dst) noexcept {
		glBlendFunc(static_cast<GLenum>(src), static_cast<GLenum>(dst));
	}

	enum class primitive : GLenum {
		points         = GL_POINTS,
		line_strip     = GL_LINE_STRIP,
		line_loop      = GL_LINE_LOOP,
		lines	       = GL_LINES,
		triangle_strip = GL_TRIANGLE_STRIP,
		triangle_fan   = GL_TRIANGLE_FAN,
		triangles      = GL_TRIANGLES
		// adjacency is useless
		// patches is missing
	};

	enum class index_type : GLenum {
		unsigned_byte  = GL_UNSIGNED_BYTE,
		unsigned_short = GL_UNSIGNED_SHORT,
		unsigned_int   = GL_UNSIGNED_INT
	};

	inline void draw(primitive mode, GLsizei vertex_count, index_type type, auto offset = 0) noexcept {
		glDrawElements(static_cast<GLenum>(mode), vertex_count, static_cast<GLenum>(type), reinterpret_cast<void *>(offset));
	}

	namespace detail {

		template<GLenum cap>
		inline void capability(bool enable) noexcept {
			if(enable)
				glEnable(cap);
			else
				glDisable(cap);
		}

	}

	inline constexpr auto && set_viewport 	      = glViewport;
	inline constexpr auto && set_background_color = glClearColor;
	inline constexpr auto && depth_testing 	      = detail::capability<GL_DEPTH_TEST>; // glDepthFunc() & glDepthRange() missing
	inline constexpr auto && set_cleared_depth    = glClearDepth;
	inline constexpr auto && blending	      = detail::capability<GL_BLEND>; // glBlendEquation() & ...Seperate() versions missing

	// smoothing missing (GL_XXX_SMOOTH + GL_BLEND + glBlendFunc() + glPointSize()/glLineWidth())
	// offsets missing (GL_POLYGON_OFFSET_XXX + glPolygonOffset())
	// stencil testing missing (GL_STENCIL_TEST + glStencilFunc() + glStencilOp() + glClearStencil())

}

#endif // _GFX_GL_DRAWING_HPP_
