#ifndef _GFX_SPRITES_HPP_
#define _GFX_SPRITES_HPP_

#include <stb_image.h>
#include "quads.hpp"
#include "opengl/textures.hpp"

namespace gfx {

	class image {
	public:
		explicit image(char const * file) {
			stbi_set_flip_vertically_on_load(true);
			data = stbi_load(file, &width, &height, &channel_count, 0);
			if(!data)
				throw gl::error{"Error loading image"};
		}

		~image() { stbi_image_free(data); }
		constexpr gl::image_format format() const {
			switch(channel_count) {
			case 3:	 return gl::image_format::rgb;
			case 4:	 return gl::image_format::rgba;
			}
			throw gl::error{"Unsupported bytes per pixel"};
		}

	public:
		int width, height, channel_count;
		unsigned char * data;
	};

	class sprite {
	public:
		sprite(auto const & img, bool pixelated = false, bool mipmap = true) noexcept
		: texture_{gl::texture_target::_2d} {
			texture_.bind();
			gl::texture2d(img.width, img.height, img.format(), gl::image_type::unsigned_byte, img.data, gl::image_format::rgba);
			// no wrapping possible
			// defaults: mag = linear & min = nearest on linear
			if(pixelated)
				texture_.mag_filter(gl::texture_filtering::nearest);
			if(mipmap) {
				texture_.mipmap();
				texture_.min_filter(gl::texture_mipmap_filtering::linear_on_linear);
				return;
			}
			texture_.min_filter(gl::texture_filtering::nearest);
		}

		void use() const noexcept { texture_.bind(); }
	private:
		gl::texture texture_;
	};

	class sprite_renderer : public quad_renderer<sprite_renderer> {
	friend quad_renderer;
	public:
		sprite_renderer(gl::shader const & vert) noexcept
		: quad_renderer{vert, SPRITE_FRAG} {}

		void set_alpha_treshold(float thresh) const noexcept {
			gl::set_uniform_float(1, thresh);
		}

	private:
		void prepare() const noexcept { gl::set_active_texture_unit(0); }
	};

	class spritesheet_renderer : public quad_renderer<spritesheet_renderer> {
	friend quad_renderer;
	public:
		spritesheet_renderer(gl::shader const & vert) noexcept
		: quad_renderer{vert, SPRITESHEET_FRAG} {}

		void set_alpha_treshold(float thresh) const noexcept {
			gl::set_uniform_float(1, thresh);
		}

		void set_sprite_position(float x, float y) const noexcept {
			gl::set_uniform_2_floats(2, x, y);
		}

		void set_sprite_relative_size(float x, float y) const noexcept {
			gl::set_uniform_2_floats(3, x, y);
		}

	private:
		void prepare() const noexcept { gl::set_active_texture_unit(0); }
	};

}

#endif // _GFX_SPRITES_HPP_
