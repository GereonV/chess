#include <cstdlib> // std::getenv()
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
		change = true;
		board.set_flipped(bs.flipped);
	};
	menu(bs, light_color_updated, dark_color_updated, flipped);
	if(change)
		write_config(config{bs});
}

int main() {
	auto config = read_config();
	board_settings b_settings{config.board}; // default flipped
	gfx::context ctx{"Schach", 1280, 720};
	ctx.clear_to(.12f, .12f, .12f);
	gfx::imgui::context imctx{ctx.window};
	ImGui::GetIO().IniFilename = nullptr;
	auto vert = gfx::quad_vertex_shader();
	board_renderer board{vert, b_settings};
	vert.~shader();
	render_settings r_settings; // default size
	while(ctx.update([&](auto width, auto height) {
		r_settings.scale_for(width, height);
		gfx::imgui::frame _{};
		board.use();
		update_settings(b_settings, board);
		render(r_settings);
	}));
}

#ifdef _WIN32
int WinMain() { main(); }
#endif // _WIN32
