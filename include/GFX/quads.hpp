#ifndef _GFX_QUADS_HPP_
#define _GFX_QUADS_HPP_

#include "gfx.hpp"
#include "opengl/buffers.hpp"
#include "opengl/shaders.hpp"

namespace gfx {

	inline gl::shader quad_vertex_shader() noexcept {
		gl::shader vert{gl::shader_type::vertex, QUAD_VERT};
		vert.compile();
		return vert;
	}

	template<typename T>
	class quad_renderer {
	friend T;
	private:
		quad_renderer(gl::shader const & vert, char const * source_frag, bool centered_coords = false) noexcept {
			static constexpr float vertices[] {
			// 	pos        coords
				-.5f, -.5f, 0, 0, // lower left
				 .5f, -.5f, 1, 0, // lower right
				 .5f,  .5f, 1, 1, // top right
				-.5f,  .5f, 0, 1  // top left
			};
			static constexpr unsigned char indices[] {
				0, 1, 2,
				0, 3, 2
			};
			vao_.bind();
			bo_.bind();
			bo_.vbo().buffer_data(vertices, gl::data_store_usage::static_draw);
			bo_.ebo().buffer_data( indices, gl::data_store_usage::static_draw);
			gl::vertex_attrib_pointer pos_ptr{0}, coord_ptr{1};
			vao_.enable_attrib_pointers(pos_ptr, coord_ptr);
			pos_ptr  .set(2, gl::data_type::_float, false, 4 * sizeof(float), 0);
			coord_ptr.set(2, gl::data_type::_float, false, 4 * sizeof(float), centered_coords ? 0 : 2 * sizeof(float));
			gl::shader frag{gl::shader_type::fragment, source_frag};
			frag.compile();
			program_.attach(vert, frag);
			program_.link();
		}

		void prepare() const noexcept {}
	public:
		void use() const noexcept {
			vao_.bind();
			program_.use();
			static_cast<T const *>(this)->prepare();
		}

	private:
		gl::vertex_array_object vao_;
		gl::buffer_object bo_; // kept around for deletion
		gl::shader_program program_;
	};

	inline void set_transformation(matrix const & mat) noexcept {
		gl::set_uniform_4_mats(0, 1, false, mat[0]);
	}

	inline void draw_quad() noexcept {
		gl::draw(gl::primitive::triangles, 6, gl::index_type::unsigned_byte, 0);
	}

}

#endif // _GFX_QUADS_HPP_
