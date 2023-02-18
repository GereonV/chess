#ifndef _GFX_GFX_HPP_
#define _GFX_GFX_HPP_

#include <cmath>
#include "opengl/loader.hpp"
#include "opengl/drawing.hpp"
#include "shadersrc.hpp"

namespace gfx {

	using matrix = float [4][4];

	constexpr matrix & operator|(matrix & mat, auto f) noexcept {
		f(mat);
		return mat;
	}

	inline constexpr auto identity = [](matrix & mat) noexcept {
		mat[0][0] = mat[1][1] = mat[2][2] = mat[3][3] = 1;
			    mat[1][0] = mat[2][0] = mat[3][0] =
		mat[0][1] 	      = mat[2][1] = mat[3][1] =
		mat[0][2] = mat[1][2] 		  = mat[3][2] =
		mat[0][3] = mat[1][3] = mat[2][3]	      = 0;
	};

	constexpr auto scale(float x, float y) noexcept {
		return [=](matrix & mat) noexcept {
			auto column = [&](auto i) {
				mat[i][0] *= x;
				mat[i][1] *= y;
			};
			column(0);
			column(1);
			column(2);
			column(3);
		};
	}

	constexpr auto scale(float fac) noexcept { return scale(fac, fac); }

	constexpr auto rotate(float alpha) noexcept {
		return [=](matrix & mat) noexcept {
			auto column = [&mat, s = std::sin(alpha), c = std::cos(alpha)](auto i) {
				auto a = mat[i][0], b = mat[i][1];
				mat[i][0] = b * s + a * c;
				mat[i][1] = b * c - a * s;
			};
			column(0);
			column(1);
			column(2);
			column(3);
		};
	}

	constexpr auto translate(float x, float y, float z = 0) noexcept {
		return [=](matrix & mat) noexcept {
			auto column = [&](auto i) {
				mat[i][0] += mat[i][3] * x;
				mat[i][1] += mat[i][3] * y;
				mat[i][2] += mat[i][3] * z;
			};
			column(0);
			column(1);
			column(2);
			column(3);
		};
	}

	class context {
	public:
		context(char const * name)
		: window_{creator(), name} { init(); }

		context(char const * name, int width, int height)
		: window_{creator(), name, width, height} { init(); }

		void clear_to(float r, float g, float b, float a = 1) noexcept {
			gl::set_background_color(r, g, b, a);
		}

		void focus() { window_.make_current(); }
		auto time() const noexcept { return window_.owner().time(); }
		[[nodiscard]] bool update(auto f) {
			static gl::clearer clear{true, true};
			auto [w, h] = window_.size();
			gl::set_viewport(0, 0, w, h);
			clear();
			f(w, h);
			window_.swap_buffers();
			window_.owner().poll();
			return !window_.should_close();
		}

#ifdef _DEBUG
		void wireframe() const noexcept { gl::set_polygon_mode(gl::polygon_mode::line); }
		void normal() const noexcept { gl::set_polygon_mode(gl::polygon_mode::fill); }
#endif

	private:
		static std::shared_ptr<gl::creator const> creator() noexcept {
			if(!creator_.expired())
				return creator_.lock();
			auto ptr = std::make_shared<gl::creator>();
			// ptr->set_hint(gl::hint::translucent); // HUGE performance loss
			creator_ = ptr;
			return ptr;
		}

		void init() {
			window_.make_current();
			gl::disable_vsync();
			gl::load();
			gl::depth_testing(true);
			gl::blending(true);
			gl::set_blending(gl::blending_factor::src_alpha, // use alpha
				gl::blending_factor::inv_src_alpha); // fade other fragments
		}

	private:
		static inline constinit std::weak_ptr<gl::creator const> creator_{};
		gl::window window_;
	};

}

#endif // _GFX_GFX_HPP_
