#ifndef _GFX_GL_BUFFERS_HPP_
#define _GFX_GL_BUFFERS_HPP_

#include "loader.hpp"

#include <span>

/*
 * GL_ARRAY_BUFFER 		Vertex attributes
 * GL_ATOMIC_COUNTER_BUFFER 	Atomic counter storage
 * GL_COPY_READ_BUFFER 		Buffer copy source
 * GL_COPY_WRITE_BUFFER 	Buffer copy destination
 * GL_DISPATCH_INDIRECT_BUFFER 	Indirect compute dispatch commands
 * GL_DRAW_INDIRECT_BUFFER 	Indirect command arguments
 * GL_ELEMENT_ARRAY_BUFFER 	Vertex array indices
 * GL_PIXEL_PACK_BUFFER 	Pixel read target
 * GL_PIXEL_UNPACK_BUFFER 	Texture data source
 * GL_QUERY_BUFFER 		Query result buffer
 * GL_SHADER_STORAGE_BUFFER 	Read-write storage for shaders
 * GL_TEXTURE_BUFFER 		Texture data buffer
 * GL_TRANSFORM_FEEDBACK_BUFFER	Transform feedback buffer
 * GL_UNIFORM_BUFFER 		Uniform block storage
 */

namespace gfx::gl {

	enum class data_store_usage : GLenum {
		stream_draw  = GL_STREAM_DRAW,
		stream_read  = GL_STREAM_READ,
		stream_copy  = GL_STREAM_COPY,
		static_draw  = GL_STATIC_DRAW,
		static_read  = GL_STATIC_READ,
		static_copy  = GL_STATIC_COPY,
		dynamic_draw = GL_DYNAMIC_DRAW,
		dynamic_read = GL_DYNAMIC_READ,
		dynamic_copy = GL_DYNAMIC_COPY
	};

	enum class data_type : GLenum {
		byte 	       = GL_BYTE,
		unsigned_byte  = GL_UNSIGNED_BYTE,
		_short 	       = GL_SHORT,
		unsigned_short = GL_UNSIGNED_SHORT,
		_int 	       = GL_INT,
		unsigned_int   = GL_UNSIGNED_INT,
		half_float     = GL_HALF_FLOAT,
		_float 	       = GL_FLOAT,
		_double        = GL_DOUBLE,
		fixed 	       = GL_FIXED
	};

	struct buffer {
	public:
		template<auto N>
		constexpr buffer(auto (&arr)[N]) noexcept : data{arr}, size{sizeof(arr)} {}

		constexpr buffer(auto ptr, auto size) noexcept
		: data{ptr}, size{static_cast<GLsizeiptr>(size)} {}

		template<typename T, std::size_t E>
		constexpr buffer(std::span<T, E> span) noexcept
		: data{span.data()}, size{static_cast<GLsizeiptr>(span.size_bytes())} {}

	public:
		GLvoid const * data;
		GLsizeiptr size;
	};

	class buffer_name {
	friend class buffer_object;
	private:
		buffer_name() = default;
		constexpr buffer_name(GLuint name) noexcept : bo_{name} {}
	public:
		void buffer_data(buffer data, data_store_usage usage) noexcept {
			glNamedBufferData(bo_, data.size, data.data, static_cast<GLenum>(usage));
		}

		void sub_data(buffer data, GLintptr offset = 0) noexcept {
			glNamedBufferSubData(bo_, offset, data.size, data.data);
		}

	private:
		GLuint bo_;
	};

	class buffer_object {
	public:
		buffer_object() noexcept { glCreateBuffers(2, &vbo_); }
		buffer_object(buffer_object const &) = delete;
		constexpr buffer_object(buffer_object && other) noexcept
		: vbo_{std::exchange(other.vbo_, 0)}, ebo_{std::exchange(other.ebo_, 0)} {}

		~buffer_object() { glDeleteBuffers(2, &vbo_); }
		buffer_name vbo() const noexcept { return vbo_; }
		buffer_name ebo() const noexcept { return ebo_; }
		void bind() const noexcept {
			glBindBuffer(GL_ARRAY_BUFFER, vbo_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		}

	private:
		GLuint vbo_, ebo_;
	};

	class vertex_attrib_pointer {
	friend class vertex_array_object;
	public:
		constexpr vertex_attrib_pointer(GLuint index) noexcept : index_{index} {}
		void enable() const noexcept { glEnableVertexAttribArray(index_); }
		void disable() const noexcept { glDisableVertexAttribArray(index_); }
		void set(GLint size, data_type type, GLboolean normalized, GLsizei stride, auto offset) noexcept {
			glVertexAttribPointer(index_, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void *>(offset));
		}

	private:
		GLuint index_;
	};

	class vertex_array_object {
	public:
		vertex_array_object() noexcept { glCreateVertexArrays(1, &vao_); }
		vertex_array_object(vertex_array_object const &) = delete;
		constexpr vertex_array_object(vertex_array_object && other) noexcept
		: vao_{std::exchange(other.vao_, 0)} {}

		~vertex_array_object() { glDeleteVertexArrays(1, &vao_); }
		void bind() const noexcept { glBindVertexArray(vao_); }
		void enable_attrib_pointers(auto const &... ptrs) noexcept {
			(glEnableVertexArrayAttrib(vao_, static_cast<vertex_attrib_pointer>(ptrs).index_), ...);
		}

		void disable_attrib_pointers(auto const &... ptrs) noexcept {
			(glDisableVertexArrayAttrib(vao_, static_cast<vertex_attrib_pointer>(ptrs).index_), ...);
		}

	private:
		GLuint vao_;
	};

}

#endif // _GFX_GL_BUFFERS_HPP_
