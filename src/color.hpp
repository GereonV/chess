#ifndef CHESS_COLOR_HPP
#define CHESS_COLOR_HPP

#include <ios>
#include <iomanip>
#include <stdexcept>
#include <GAT/chars.hpp>
#include <GAT/combinators.hpp>

struct color {
private:
	using      data = float[3];
	using       ref = data       &;
	using const_ref = data const &;
public:
	color() = default;
	constexpr color(float r, float g, float b) : _rgb{r, g, b} {}
	constexpr void r(float r) noexcept { _rgb[0] = r; }
	constexpr void g(float g) noexcept { _rgb[1] = g; }
	constexpr void b(float b) noexcept { _rgb[2] = b; }
	constexpr void set(float r, float g, float b) noexcept {
		_rgb[0] = r;
		_rgb[1] = g;
		_rgb[2] = b;
	}

	constexpr float r() const noexcept { return _rgb[0]; }
	constexpr float g() const noexcept { return _rgb[1]; }
	constexpr float b() const noexcept { return _rgb[2]; }
	constexpr       ref rgb()       noexcept { return _rgb; }
	constexpr const_ref rgb() const noexcept { return _rgb; }
	constexpr operator       ref()       noexcept { return _rgb; }
	constexpr operator const_ref() const noexcept { return _rgb; }
private:
	data _rgb;
};

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
	in.read(buf, 8);
	if(!in || !sequence<exact<'0'>, left<set<'x', 'X'>, exactly<6, xdigit>>>(buf))
		throw std::invalid_argument{"Couldn't parse Color"};
	fmt(0);
	fmt(1);
	fmt(2);
	return in;
}

#endif // CHESS_COLOR_HPP
