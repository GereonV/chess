#ifndef CHESS_MENU_HPP
#define CHESS_MENU_HPP

#include <GFX/opengl/loader.hpp>
#include "structs.hpp"

inline constexpr int color_edit_flags = ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs;

inline void menu(board_settings & s, auto && light_color_updated, auto && dark_color_updated, auto && flipped) {
	auto flip = [&] {
		s.flipped = !s.flipped;
		flipped();
	};
	if(ImGui::BeginMainMenuBar()) {
		if(ImGui::BeginMenu("View")) {
			ImGui::Text("Light Color:"); ImGui::SameLine();
			if(ImGui::ColorEdit3("Light Color:", s.light_color, color_edit_flags))
				light_color_updated();
			ImGui::Text("Dark Color: "); ImGui::SameLine();
			if(ImGui::ColorEdit3("Dark Color:", s.dark_color, color_edit_flags))
				dark_color_updated();
			if(ImGui::Button("Flip"))
				flip();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if(ImGui::IsKeyPressed(ImGuiKey_F, false))
		flip();
}

#endif // CHESS_MENU_HPP
