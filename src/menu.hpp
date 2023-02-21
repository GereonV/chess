#ifndef CHESS_MENU_HPP
#define CHESS_MENU_HPP

#include <GFX/opengl/loader.hpp>

inline constexpr int color_edit_flags = ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs;

enum class settings_change {
	none,
	light_color,
	dark_color,
	flip
};

[[nodiscard]] inline settings_change menu(board_settings & s) {
	auto change = settings_change::none;
	auto flip = [&] {
		s.flipped = !s.flipped;
		change = settings_change::flip;
	};
	if(ImGui::BeginMainMenuBar()) {
		if(ImGui::BeginMenu("View")) {
			ImGui::Text("Light Color:"); ImGui::SameLine();
			if(ImGui::ColorEdit3("Light Color:", s.light_color, color_edit_flags))
				change = settings_change::light_color;
			ImGui::Text("Dark Color: "); ImGui::SameLine();
			if(ImGui::ColorEdit3("Dark Color:", s.dark_color, color_edit_flags))
				change = settings_change::dark_color;
			if(ImGui::Button("Flip"))
				flip();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if(change == settings_change::none && ImGui::IsKeyPressed(ImGuiKey_F, false))
		flip();
	return change;
}

#endif // CHESS_MENU_HPP
