#ifndef _GFX_GL_SHADERS_HPP_
#define _GFX_GL_SHADERS_HPP_

#include "loader.hpp"

namespace gfx::gl {

	struct shader_error : std::exception {
		char const * what() const noexcept override { return msg; }
		static constexpr std::size_t buf_size{512};
		char msg[buf_size];
	};

	enum class shader_type : unsigned int {
		vertex 	        = GL_VERTEX_SHADER,
		fragment        = GL_FRAGMENT_SHADER,
		geometry        = GL_GEOMETRY_SHADER,
		compute         = GL_COMPUTE_SHADER,
		tess_control 	= GL_TESS_CONTROL_SHADER,
		tess_evaluation = GL_TESS_EVALUATION_SHADER
	};

	class shader {
	friend class shader_program;
	public:
		shader(shader_type type, char const * source) noexcept
		: shader_{glCreateShader(static_cast<GLenum>(type))} {
			glShaderSource(shader_, 1, &source, 0);
		}

		shader(shader const &) = delete;
		constexpr shader(shader && other) noexcept : shader_{std::exchange(other.shader_, 0)} {}
		~shader() { glDeleteShader(shader_); }
		void compile() {
			glCompileShader(shader_);
			GLint success;
			glGetShaderiv(shader_, GL_COMPILE_STATUS, &success);
			if(success)
				return;
			shader_error err;
			glGetShaderInfoLog(shader_, err.buf_size, nullptr, err.msg);
			throw err;
		}

	private:
		GLuint shader_;
	};

	class shader_program {
	public:
		shader_program() noexcept : program_{glCreateProgram()} {}
		constexpr shader_program(shader_program && other) noexcept : program_{std::exchange(other.program_, 0)} {}
		shader_program(shader_program const &) = delete;
		~shader_program() { glDeleteProgram(program_); }
		void use() const noexcept { glUseProgram(program_); }
		void attach(auto const &... shaders) noexcept {
			(glAttachShader(program_, static_cast<shader const &>(shaders).shader_), ...);
		}

		void link() {
			glLinkProgram(program_);
			GLint success;
			glGetProgramiv(program_, GL_LINK_STATUS, &success);
			if(success)
				return;
			shader_error err;
			glGetProgramInfoLog(program_, err.buf_size, nullptr, err.msg);
			throw err;
		}

		GLint uniform(char const * name) const {
			if(auto location = glGetUniformLocation(program_, name); location != -1)
				return location;
			throw error{"Invalid uniform name"};
		}

	private:
		GLuint program_;
	};

	// glGetProgramBinary() for precompilation

	// non-square matrices missing
	inline constexpr auto && set_uniform_float  	    	 = glUniform1f;
	inline constexpr auto && set_uniform_2_floats	     	 = glUniform2f;
	inline constexpr auto && set_uniform_3_floats 	     	 = glUniform3f;
	inline constexpr auto && set_uniform_4_floats 	     	 = glUniform4f;
	inline constexpr auto && set_uniform_int  	     	 = glUniform1i;
	inline constexpr auto && set_uniform_2_ints 	     	 = glUniform2i;
	inline constexpr auto && set_uniform_3_ints 	     	 = glUniform3i;
	inline constexpr auto && set_uniform_4_ints 	     	 = glUniform4i;
	inline constexpr auto && set_uniform_unsigned_int    	 = glUniform1ui;
	inline constexpr auto && set_uniform_2_unsigned_ints 	 = glUniform2ui;
	inline constexpr auto && set_uniform_3_unsigned_ints 	 = glUniform3ui;
	inline constexpr auto && set_uniform_4_unsigned_ints 	 = glUniform4ui;
	inline constexpr auto && set_uniform_float_arr	     	 = glUniform1fv;
	inline constexpr auto && set_uniform_2_floats_arr    	 = glUniform2fv;
	inline constexpr auto && set_uniform_3_floats_arr    	 = glUniform3fv;
	inline constexpr auto && set_uniform_4_floats_arr    	 = glUniform4fv;
	inline constexpr auto && set_uniform_int_arr	     	 = glUniform1iv;
	inline constexpr auto && set_uniform_2_ints_arr      	 = glUniform2iv;
	inline constexpr auto && set_uniform_3_ints_arr      	 = glUniform3iv;
	inline constexpr auto && set_uniform_4_ints_arr      	 = glUniform4iv;
	inline constexpr auto && set_uniform_unsigned_int_arr	 = glUniform1uiv;
	inline constexpr auto && set_uniform_2_unsigned_ints_arr = glUniform2uiv;
	inline constexpr auto && set_uniform_3_unsigned_ints_arr = glUniform3uiv;
	inline constexpr auto && set_uniform_4_unsigned_ints_arr = glUniform4uiv;
	inline constexpr auto && set_uniform_2_mats    	  	 = glUniformMatrix2fv;
	inline constexpr auto && set_uniform_3_mats    	  	 = glUniformMatrix3fv;
	inline constexpr auto && set_uniform_4_mats    	  	 = glUniformMatrix4fv;

}

#endif // _GFX_GL_SHADERS_HPP_
