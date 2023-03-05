#ifndef CHESS_RENDERING_HPP
#define CHESS_RENDERING_HPP

#include <GFX/sprites.hpp>
#include "shadersrc.hpp"
#include "structs.hpp"

struct spritesheet {
public:
	explicit spritesheet(std::span<unsigned char const> s) {
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load_from_memory(s.data(), static_cast<int>(s.size_bytes()), &width, &height, &channel_count, 0);
		if(!data)
			throw gfx::gl::error{stbi_failure_reason()};
	}

	~spritesheet() { stbi_image_free(data); }
	constexpr gfx::gl::image_format format() const {
		switch(channel_count) {
		case 3:	 return gfx::gl::image_format::rgb;
		case 4:	 return gfx::gl::image_format::rgba;
		}
		throw gfx::gl::error{"Unsupported bytes per pixel"};
	}

public:
	int width, height, channel_count;
	unsigned char * data;
};

struct render_settings {
	int width, height;
	float size{1.5f}; // size=1 -> half screen, size=2 -> fullscreen
	piece active_piece{};

	bool operator==(render_settings const &) const = default;
	constexpr void scaling(float & x, float & y) const noexcept {
		x = y = size;
		if(width > height)
			x *= static_cast<float>(height) / static_cast<float>(width);
		else
			y *= static_cast<float>(width) / static_cast<float>(height);
	}

	[[nodiscard]] ImVec2 mouse(float x, float y) const noexcept {
		auto mouse = ImGui::GetMousePos();
		// ((mx + 0.5) / width - 0.5) / (scalex / 2)
		// (0.5 - (my + 0.5) / height) / (scaley / 2)
		mouse.x = 2 * ((mouse.x + .5f) / width - .5f) / x;
		mouse.y = 2 * (.5f - (mouse.y + .5f) / height) / y;
		return mouse;
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

class piece_renderer : public gfx::sprite_renderer {
using gfx::sprite_renderer::sprite_renderer;
public:
	static piece_renderer create() noexcept {
		gfx::gl::shader vert{gfx::gl::shader_type::vertex, PIECES_VERT};
		vert.compile();
		piece_renderer pieces{vert};
		unsigned char vertices[64 * 4 * 2]; // 64 squares * 4 vertices/square * 2 bytes/vertex
		unsigned char indices[64 * 6] { // 64 squares * 6 elements/square
			  0,   1,   2,   0,   3,   2,
			  4,   5,   6,   4,   7,   6,
			  8,   9,  10,   8,  11,  10,
			 12,  13,  14,  12,  15,  14,
			 16,  17,  18,  16,  19,  18,
			 20,  21,  22,  20,  23,  22,
			 24,  25,  26,  24,  27,  26,
			 28,  29,  30,  28,  31,  30,
			 32,  33,  34,  32,  35,  34,
			 36,  37,  38,  36,  39,  38,
			 40,  41,  42,  40,  43,  42,
			 44,  45,  46,  44,  47,  46,
			 48,  49,  50,  48,  51,  50,
			 52,  53,  54,  52,  55,  54,
			 56,  57,  58,  56,  59,  58,
			 60,  61,  62,  60,  63,  62,
			 64,  65,  66,  64,  67,  66,
			 68,  69,  70,  68,  71,  70,
			 72,  73,  74,  72,  75,  74,
			 76,  77,  78,  76,  79,  78,
			 80,  81,  82,  80,  83,  82,
			 84,  85,  86,  84,  87,  86,
			 88,  89,  90,  88,  91,  90,
			 92,  93,  94,  92,  95,  94,
			 96,  97,  98,  96,  99,  98,
			100, 101, 102, 100, 103, 102,
			104, 105, 106, 104, 107, 106,
			108, 109, 110, 108, 111, 110,
			112, 113, 114, 112, 115, 114,
			116, 117, 118, 116, 119, 118,
			120, 121, 122, 120, 123, 122,
			124, 125, 126, 124, 127, 126,
			128, 129, 130, 128, 131, 130,
			132, 133, 134, 132, 135, 134,
			136, 137, 138, 136, 139, 138,
			140, 141, 142, 140, 143, 142,
			144, 145, 146, 144, 147, 146,
			148, 149, 150, 148, 151, 150,
			152, 153, 154, 152, 155, 154,
			156, 157, 158, 156, 159, 158,
			160, 161, 162, 160, 163, 162,
			164, 165, 166, 164, 167, 166,
			168, 169, 170, 168, 171, 170,
			172, 173, 174, 172, 175, 174,
			176, 177, 178, 176, 179, 178,
			180, 181, 182, 180, 183, 182,
			184, 185, 186, 184, 187, 186,
			188, 189, 190, 188, 191, 190,
			192, 193, 194, 192, 195, 194,
			196, 197, 198, 196, 199, 198,
			200, 201, 202, 200, 203, 202,
			204, 205, 206, 204, 207, 206,
			208, 209, 210, 208, 211, 210,
			212, 213, 214, 212, 215, 214,
			216, 217, 218, 216, 219, 218,
			220, 221, 222, 220, 223, 222,
			224, 225, 226, 224, 227, 226,
			228, 229, 230, 228, 231, 230,
			232, 233, 234, 232, 235, 234,
			236, 237, 238, 236, 239, 238,
			240, 241, 242, 240, 243, 242,
			244, 245, 246, 244, 247, 246,
			248, 249, 250, 248, 251, 250,
			252, 253, 254, 252, 255, 254,
		};
		pieces.bo_.vbo().buffer_data(vertices, gfx::gl::data_store_usage::dynamic_draw);
		pieces.bo_.ebo().buffer_data( indices, gfx::gl::data_store_usage:: static_draw);
		gfx::gl::vertex_attrib_pointer{0}.set_int(2, gfx::gl::data_type::unsigned_byte, 2, 0);
		return pieces;
	}

	void update_vertices(unsigned char const (&vertices)[64 * 4 * 2], unsigned short num_vertices) const noexcept {
		bo_.vbo().sub_data({vertices, 2 * num_vertices});
	}

	void set_custom_position(float x, float y) const noexcept {
		gfx::gl::set_uniform_2_floats(2, x, y);
	}

	void set_spritesheet_settings(float const (&texture_coordinates)[7][2][2]) const noexcept {
		gfx::gl::set_uniform_2_floats_arr(3, 14, texture_coordinates[0][0]);
	}

};

// after board_renderer::use()
inline void render(render_settings const & settings, piece_renderer const & pieces) noexcept {
	static constinit render_settings old{1, 1, 0};
	static constinit gfx::matrix transform{
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	auto redo_trans = (settings != old);
	if(redo_trans) {
		old = settings;
		settings.scaling(transform[0][0], transform[1][1]);
		gfx::set_transformation(transform);
	}
	gfx::draw_quad();
	pieces.use();
	if(redo_trans)
		gfx::set_transformation(transform);
	unsigned char num_pieces{};
	// square index: 1 bit right, 1 bit top, 3 bits rank, 3 bits file
	// piece: 1 bit enabling custom position, 1 bit dark, 6 bits piece type
	unsigned char vertices[64 * 4 * 2]; // 64 squares * 4 vertices/square * 2 bytes/vertex
	auto add = [&](unsigned char square, unsigned char piece) {
		auto i = num_pieces++ * 2 * 4;
		vertices[i++] = (0b00 << 6) | square;
		vertices[i++] = piece;
		vertices[i++] = (0b10 << 6) | square;
		vertices[i++] = piece;
		vertices[i++] = (0b11 << 6) | square;
		vertices[i++] = piece;
		vertices[i++] = (0b01 << 6) | square;
		vertices[i++] = piece;
	};
	// TODO fill vertices
	if(auto data = settings.active_piece.bits(); data) {
		auto [x, y] = settings.mouse(transform[0][0], transform[1][1]);
		add(0, (1 << 7) | data);
		pieces.set_custom_position(x, y);
	}
	pieces.update_vertices(vertices, 4 * num_pieces);
	gfx::gl::draw(gfx::gl::primitive::triangles, 6 * num_pieces, gfx::gl::index_type::unsigned_byte, 0); // 6 elements/piece
}

#endif // CHESS_RENDERING_HPP
