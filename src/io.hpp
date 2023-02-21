#ifndef CHESS_IO_HPP
#define CHESS_IO_HPP

#ifdef _DEBUG
#include <iostream>
#define ERR(e) std::cerr << e.what() << '\n'
#else
#define ERR(e)
#endif

#include <filesystem>
#include <fstream>
#include "color.hpp"
#include "structs.hpp"

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

[[nodiscard]] inline std::filesystem::path config_path() noexcept {
	return get_app_dir() /= "config";
}

inline std::ostream & operator<<(std::ostream & out, config const & config) noexcept {
	return out << "light=" << config.board.light_color << '\n'
		<< "dark=" << config.board.dark_color << '\n';
}

inline void write_config(config const & config) noexcept {
	std::ofstream{config_path(), std::ios_base::trunc} << config;
}

inline void read_config_impl(std::istream && in, config & config) noexcept try {
	char buf[6];
	do {
		if(!in.getline(buf, 6, '='))
			break;
		if(buf == std::string_view{"light"})
			in >> config.board.light_color;
		else if(buf == std::string_view{"dark"})
			in >> config.board.dark_color;
		else
			break;
	} while(in.get() == '\n');
} catch([[maybe_unused]] std::invalid_argument const & e) {
	ERR(e);
}

inline config read_config() noexcept {
	config config;
	auto path = config_path();
	if(std::filesystem::exists(path))
		read_config_impl(std::ifstream{path}, config);
	return config;
}

#endif // CHESS_IO_HPP
