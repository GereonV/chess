#include "rendering.hpp"

inline constinit board_view view{
	{.6f, .6f, .7f},
	{.35f, .35f, .5f},
	true
};

inline constexpr int color_edit_flags = ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs;

int main() {
	gfx::context ctx{"Schach", 1280, 720};
	ctx.clear_to(.12f, .12f, .12f);
	gfx::imgui::context imctx{ctx.window};
	ImGui::GetIO().IniFilename = nullptr;
	auto vert = gfx::quad_vertex_shader();
	board_renderer board{vert};
	vert.~shader();
	gfx::matrix transform;
	while(ctx.update([&](auto width, auto height) {
		gfx::imgui::frame _{};
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("View")) {
				ImGui::Text("Light Color:"); ImGui::SameLine();
				ImGui::ColorEdit3("Light Color:", view.light_color, color_edit_flags);
				ImGui::Text("Dark Color: "); ImGui::SameLine();
				ImGui::ColorEdit3("Dark Color:", view.dark_color, color_edit_flags);
				if(ImGui::Button("Flip"))
					view.flipped = !view.flipped;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		auto w = width > height ? static_cast<float>(height) / static_cast<float>(width) : 1;
		auto h = height > width ? static_cast<float>(width) / static_cast<float>(height) : 1;
		board.use();
		board.set_white_color(view.light_color);
		board.set_black_color(view.dark_color);
		board.set_flipped(view.flipped);
		gfx::set_transformation(transform |
			gfx::identity |
			gfx::scale(w, h) |
			gfx::scale(1.5f));
		gfx::draw_quad();
	}));
}

#ifdef _WIN32
int WinMain() { main(); }
#endif // _WIN32
