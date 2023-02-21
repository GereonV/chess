#include <cstdlib> // std::getenv()
#include "rendering.hpp"
#include "settings.hpp"

inline board_view view;

inline constexpr int color_edit_flags = ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs;

int main() {
	auto s = read_settings();
	view.light_color = s.light_color;
	view.dark_color = s.dark_color;
	gfx::context ctx{"Schach", 1280, 720};
	ctx.clear_to(.12f, .12f, .12f);
	gfx::imgui::context imctx{ctx.window};
	ImGui::GetIO().IniFilename = nullptr;
	auto vert = gfx::quad_vertex_shader();
	board_renderer board{vert, view};
	vert.~shader();
	render_settings settings;
	while(ctx.update([&](auto width, auto height) {
		gfx::imgui::frame _{};
		settings.scale_for(width, height);
		board.use();
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("View")) {
				ImGui::Text("Light Color:"); ImGui::SameLine();
				if(ImGui::ColorEdit3("Light Color:", view.light_color, color_edit_flags))
					board.set_light_color(view.light_color);
				ImGui::Text("Dark Color: "); ImGui::SameLine();
				if(ImGui::ColorEdit3("Dark Color:", view.dark_color, color_edit_flags))
					board.set_dark_color(view.dark_color);
				if(ImGui::Button("Flip"))
					board.set_flipped(view.flipped = !view.flipped);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		render(settings, board);
	}));
}

#ifdef _WIN32
int WinMain() { main(); }
#endif // _WIN32
