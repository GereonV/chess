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
#include <iomanip>
#include <stdexcept>
#include <GAT/chars.hpp>
#include <GAT/combinators.hpp>
#include "structs.hpp"

struct config {
	board_config board;
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

// COLOR

inline std::ostream & operator<<(std::ostream & out, color const & color) noexcept {
	auto fmt = [&](float f) {
		out << std::setw(2) << static_cast<int>(255.f * f + .5f);
	};
	auto flags = out.flags();
	auto fill = out.fill();
	out << "0x" << std::hex << std::setfill('0');
	fmt(color.r());
	fmt(color.g());
	fmt(color.b());
	out.flags(flags);
	out.fill(fill);
	return out;
}

inline std::istream & operator>>(std::istream & in, color & color) {
	char buf[8];
	auto h2d = [](char c) { return c >= 'a' ? c - 'a' + 10 : c >= 'A' ? c - 'A' + 10 : c - '0'; };
	auto fmt = [&](int i) {
		auto c1 = h2d(buf[2 + 2 * i]);
		auto c2 = h2d(buf[3 + 2 * i]);
		color[i] = static_cast<float>(16 * c1 + c2) / 255.f;
	};
	using namespace gat::chars;
	using namespace gat::combinators;
	in.read(buf, 8); // potentially throwing (in.exceptions())
	if(!in || !sequence<exact<'0'>, left<set<'x', 'X'>, exactly<6, xdigit>>>(buf))
		throw std::invalid_argument{"Couldn't parse Color"};
	fmt(0);
	fmt(1);
	fmt(2);
	return in;
}

#endif // CHESS_IO_HPP
