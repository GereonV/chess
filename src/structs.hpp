#ifndef CHESS_STRUCTS_HPP
#define CHESS_STRUCTS_HPP

enum class piece_color : unsigned char {
	WHITE = 0,
	BLACK = 1 << 6,
};

enum class piece_type : unsigned char {
	NONE   =  0,
	KING   =  1,
	QUEEN  =  2,
	BISHOP =  4,
	KNIGHT =  8,
	ROOK   = 16,
	PAWN   = 32,
};

struct piece {
public:
	piece() = default;
	constexpr piece(piece_color c, piece_type t) noexcept
	: _bitfield{static_cast<unsigned char>(static_cast<int>(c) | static_cast<int>(t))} {}

	[[nodiscard]] constexpr bool is_type(piece_type type) const noexcept { return _bitfield | static_cast<int>(type); }
	[[nodiscard]] constexpr bool white() const noexcept { return !(_bitfield >> 6); }
	[[nodiscard]] constexpr bool black() const noexcept { return   _bitfield >> 6 ; }
	[[nodiscard]] constexpr unsigned char bits() const noexcept { return _bitfield; }
	[[nodiscard]] constexpr operator unsigned char() const noexcept { return _bitfield; }
private:
	unsigned char _bitfield{};
};

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

// config and session
struct board_config {
	color light_color{.6f, .6f, .7f};  // #9999B3
	color dark_color{.35f, .35f, .5f}; // #595980
};

// session view options
struct board_settings : board_config {
	bool flipped{false};
};

#endif // CHESS_STRUCTS_HPP
