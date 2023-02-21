#ifndef CHESS_SETTINGS_HPP
#define CHESS_SETTINGS_HPP

#ifdef _DEBUG
#include <iostream>
#define ERR(e) std::cerr << e.what() << '\n'
#else
#define ERR(e)
#endif

#include <filesystem>
#include <fstream>
#include <GFX/opengl/loader.hpp>
#include "color.hpp"

struct settings {
	color light_color{.6f, .6f, .7f};
	color dark_color{.35f, .35f, .5f};
};

[[nodiscard]] inline std::filesystem::path get_app_dir() noexcept try {
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
	ERR(e);
	return {};
}

inline std::ostream & operator<<(std::ostream & out, settings const & settings) noexcept {
	return out << "light=" << settings.light_color << '\n'
		<< "dark=" << settings.dark_color << '\n';
}

// TODO not really useful + unreadable
inline bool operator>>(std::istream & in, settings & settings) noexcept try {
	char buf[6]; // light
	buf[5] = '\n';
	while(!in.eof()) {
		if(buf[5] != '\n' || !in.getline(buf, 6, '='))
			return !buf[0];
		if(buf == std::string_view{"light"})
			in >> settings.light_color;
		else if(buf == std::string_view{"dark"})
			in >> settings.dark_color;
		else
			return false;
		buf[5] = static_cast<char>(in.get());
	}
	return true;
} catch(std::invalid_argument const & e) {
	ERR(e);
	return false;
}

inline std::filesystem::path config_path{get_app_dir() /= "config"};

inline void write_settings(settings const & settings) noexcept {
	std::ofstream{config_path, std::ios_base::trunc} << settings;
}

inline settings read_settings() noexcept {
	settings settings;
	if(std::filesystem::exists(config_path))
		std::ifstream{config_path} >> settings;
	return settings;
}

#endif // CHESS_SETTINGS_HPP
