#ifndef _GFX_GL_LOADER_HPP
#define _GFX_GL_LOADER_HPP

#include <exception>
#include <memory>
#include <string>
#include <utility>

namespace gfx::gl {

	class error : public std::exception {
	public:
		error(std::string && msg) noexcept : msg_{std::move(msg)} {}
		char const * what() const noexcept override { return msg_.data(); }
	private:
		std::string msg_;
	};

	inline void load();

}

#ifdef GLEW

#include <GL/glew.h>

inline void gfx::gl::load() {
	using namespace std::literals;
	if(auto err = glewInit(); err)
		throw error{"glewInit() failed: "s + reinterpret_cast<char const *>(glewGetErrorString(err))};
}

#endif // GLEW

#ifdef GLAD

#include <glad/glad.h>

inline void gfx::gl::load() {
	if(!gladLoadGL())
		throw error{"gladLoadGL() failed"};
}

#endif // GLAD

#ifdef GLFW

#include <GLFW/glfw3.h>

namespace gfx::gl {

	enum class hint : int {
		visible     = GLFW_VISIBLE,
		resizeable  = GLFW_RESIZABLE,
		decorated   = GLFW_DECORATED,
		maximized   = GLFW_MAXIMIZED,
		translucent = GLFW_TRANSPARENT_FRAMEBUFFER
	};

	class creator {
	public:
		creator() {
			if(ptr_)
				throw error{"GLFW instance already exists"};
			ptr_ = this;
			glfwSetErrorCallback([](int code, char const * description) {
				throw error{"GLFW-Error (" + std::to_string(code) + "): " + description};
			});
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true); // for Mac OS X
		}

		creator(creator const &) = delete;
		~creator() { glfwTerminate(); ptr_ = nullptr; }
		void poll() const noexcept { glfwPollEvents(); }
		void wait() const noexcept { glfwWaitEvents(); }
		void wait(double timeout) const noexcept { glfwWaitEventsTimeout(timeout); }
		void set_hint(hint hint, bool enable = true) noexcept { glfwWindowHint(static_cast<int>(hint), enable); }
		[[nodiscard]] double time() const noexcept { return glfwGetTime(); }
	private:
		static inline creator * ptr_;
	};

	class window {
	public:
		window(std::shared_ptr<creator const> creator, char const * title)
		: ctx_{std::move(creator)} {
			auto monitor = glfwGetPrimaryMonitor();
			auto mode = glfwGetVideoMode(monitor);
			win_ = glfwCreateWindow(mode->width, mode->height, title, monitor, nullptr);
		}

		window(std::shared_ptr<creator const> creator, char const * title, int width, int height)
		: ctx_{std::move(creator)}, win_{glfwCreateWindow(width, height, title, nullptr, nullptr)} {}

		window(window && other) noexcept
		: ctx_{std::move(other.ctx_)} { win_ = std::exchange(other.win_, nullptr); }

		window(window const &) = delete;
		~window() { glfwDestroyWindow(win_); }
		void destroy() { glfwDestroyWindow(win_); win_ = nullptr; }
		auto & owner() const noexcept { return *ctx_; }
		void close() noexcept { glfwSetWindowShouldClose(win_, true); }
		[[nodiscard]] bool should_close() const noexcept { return glfwWindowShouldClose(win_); }
		void make_current() { glfwMakeContextCurrent(win_); }
		[[nodiscard]] bool current() const noexcept { return glfwGetCurrentContext() == win_; }
		void swap_buffers() { glfwSwapBuffers(win_); }
		[[nodiscard]] std::pair<int, int> size() const {
			std::pair<int, int> size;
			glfwGetFramebufferSize(win_, &size.first, &size.second);
			return size;
		}

	private:
		std::shared_ptr<creator const> ctx_;
		GLFWwindow * win_;
	};

	inline void disable_vsync() { glfwSwapInterval(0); }

}

#endif // GLFW

#endif // _GFX_GL_LOADER_HPP
