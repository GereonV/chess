#include "rendering.hpp"

int main() {
	gfx::context ctx{"Schach", 1280, 720};
	ctx.clear_to(.12f, .12f, .12f);
	auto vert = gfx::quad_vertex_shader();
	board_renderer board{vert};
	board.use();
	board.set_white_color(1.f, 1.f, 1.f);
	board.view_from_black();
	vert.~shader();
	gfx::matrix transform; // for later
	while(ctx.update([&](auto width, auto height) {
		auto w = width > height ? static_cast<float>(height) / static_cast<float>(width) : 1;
		auto h = height > width ? static_cast<float>(width) / static_cast<float>(height) : 1;
		board.use();
		gfx::set_transformation(transform |
			gfx::identity |
			gfx::scale(w, h) |
			gfx::scale(1.5f));
		gfx::draw_quad();
	}));
}
