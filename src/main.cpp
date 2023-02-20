#include <cstdlib> // std::getenv()
#include <filesystem>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
#include "rendering.hpp"

inline std::filesystem::path get_app_dir() noexcept try {
	namespace fs = std::filesystem;
#ifdef _WIN32
	fs::path dir{std::getenv("APPDATA")}; // C:\Users\<username>\AppData\Roaming
	fs::create_directory(dir /= "Chess");
	return dir;
#else
	auto xdg = std::getenv("XDG_CONFIG_HOME"); // defaults to $HOME/.config
	auto home = std::getenv("HOME");  // normally /home/<username>
	auto dir = fs::path{home} /= ".config/chess";
	if(fs::exists(dir))
		return dir;
	if(xdg)
		dir.assign(xdg) /= "chess";
	fs::create_directories(dir);
	return dir;
#endif
} catch([[maybe_unused]] std::filesystem::filesystem_error const & e) {
#ifdef _DEBUG
	std::cerr << e.what() << '\n';
#endif // _DEBUG
	return {};
}

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
	board.use();
	board.set_white_color(view.light_color);
	board.set_black_color(view.dark_color);
	board.set_flipped(view.flipped);
	vert.~shader();
	gfx::matrix transform;
	while(ctx.update([&](auto width, auto height) {
		auto w = width > height ? static_cast<float>(height) / static_cast<float>(width) : 1;
		auto h = height > width ? static_cast<float>(width) / static_cast<float>(height) : 1;
		gfx::imgui::frame _{};
		board.use();
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("View")) {
				ImGui::Text("Light Color:"); ImGui::SameLine();
				if(ImGui::ColorEdit3("Light Color:", view.light_color, color_edit_flags))
					board.set_white_color(view.light_color);
				ImGui::Text("Dark Color: "); ImGui::SameLine();
				if(ImGui::ColorEdit3("Dark Color:", view.dark_color, color_edit_flags))
					board.set_black_color(view.dark_color);
				if(ImGui::Button("Flip"))
					board.set_flipped(view.flipped = !view.flipped);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
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
