#include <cstdlib> // std::getenv()
#include "io.hpp"
#include "menu.hpp"
#include "rendering.hpp"

// returns if board.use()
inline bool update_settings(board_settings & bs, board_renderer const & board) noexcept {
	auto change = menu(bs);
	if(change == settings_change::none)
		return false;
	write_config(config{bs});
	board.use();
	if(change == settings_change::flip)
		board.set_flipped(bs.flipped);
	else if(change == settings_change::light_color)
		board.set_light_color(bs.light_color);
	else
		board.set_dark_color(bs.dark_color);
	return true;
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
		if(!update_settings(b_settings, board))
			board.use();
		render(r_settings);
	}));
}

#ifdef _WIN32
int WinMain() { main(); }
#endif // _WIN32
