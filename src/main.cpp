#include "io.hpp"
#include "menu.hpp"
#include "rendering.hpp"

// after board_renderer::use();
inline void update_settings(board_settings & bs, board_renderer const & board) noexcept {
	auto change = false;
	auto light_color_updated = [&] {
		change = true;
		board.set_light_color(bs.light_color);
	};
	auto dark_color_updated = [&] {
		change = true;
		board.set_dark_color(bs.dark_color);
	};
	auto flipped = [&] {
		// change = true;
		board.set_flipped(bs.flipped);
	};
	menu(bs, light_color_updated, dark_color_updated, flipped);
	if(change)
		write_config({bs});
}

inline constinit const float tex[7][2][2]{
	// start       size
	{{   0, 0}, {204, 190}}, // king
	{{ 309, 0}, {204, 273}}, // queen
	{{ 618, 0}, {204, 228}}, // bishop
	{{ 927, 0}, {237, 304}}, // knight
	{{1236, 0}, {167, 309}}, // rook
	{{1545, 0}, {204, 248}}, // pawn

	{{0, 309}, {0, 0}}, // white, black
};

int main() {
	auto config = read_config();
	board_settings b_settings{config.board}; // not flipped by default
	gfx::context ctx{"Schach", 1280, 720};
	ctx.clear_to(.12f, .12f, .12f); // background #1E1E1F
	gfx::imgui::context imctx{ctx.window};
	auto && io = ImGui::GetIO();
	io.IniFilename = nullptr; // disable imgui.ini
	auto vert = gfx::quad_vertex_shader();
	board_renderer board{vert, b_settings};
	auto pieces = piece_renderer::create();
	pieces.use();
	pieces.set_spritesheet_settings(tex);
	gfx::sprite sheet{spritesheet{{&image_data_start, &image_data_end}}, true, false};
	vert.~shader();
	render_settings r_settings; // default size
	r_settings.active_piece = {piece_color::WHITE, piece_type::KING};
	// ctx.wireframe();
	while(ctx.update([&](auto width, auto height) {
		gfx::imgui::frame _{};
		if(!io.WantCaptureMouse)
			; // TODO is piece active?
		board.use();
		update_settings(b_settings, board);
		r_settings.width = width;
		r_settings.height = height;
		render(r_settings, pieces);
	}));
}

#ifdef _WIN32
int WinMain() { main(); }
#endif // _WIN32
