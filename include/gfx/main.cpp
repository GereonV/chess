#include <iostream>
#include "sprites.hpp"
#include "circles.hpp"

int main() try {
	gfx::context ctx{"Test Program", 720, 720};
	ctx.clear_to(.4f, .6f, 1, .05f);
	gfx::sprite wall{gfx::image{"textures/wall.jpg"}};
	gfx::sprite smiley{gfx::image{"textures/awesomeface.png"}};
	auto vert = gfx::quad_vertex_shader();
	gfx::sprite_renderer sprites{vert};
	gfx::spritesheet_renderer sheet{vert};
	gfx::circle_renderer circles{vert};
	vert.~shader();
	sheet.use();
	sheet.set_sprite_relative_size(.5f, .5f);
	circles.use();
	circles.set_color(0, 0, 0, .25f);
	gfx::matrix transform;
	unsigned long count{};
	auto begin_time = ctx.time();
	while(ctx.update([&](auto width, auto height) {
		auto sin = static_cast<float>(std::sin(ctx.time()));
		auto s = static_cast<float>(height) / static_cast<float>(width);
		sheet.use();
		wall.use();
		gfx::set_transformation(transform |
			gfx::identity |
			gfx::translate(sin / 2, 0) |
			gfx::scale(s, 1));
		gfx::draw_quad();
		sprites.use();
		smiley.use();
		gfx::set_transformation(transform |
			gfx::identity |
			gfx::scale(.5f) |
			gfx::rotate(sin * 3.1415926535f) |
			gfx::translate(0, 0, .999f * sin) |
			gfx::scale(s, 1));
		gfx::draw_quad();
		circles.use();
		gfx::set_transformation(transform |
			gfx::translate(0, -.25f, -.0001f) |
			gfx::scale(1.2f));
		gfx::draw_quad();
	})) ++count;
	auto end_time = ctx.time();
	std::cout << static_cast<double>(count) / (end_time - begin_time) << '\n';
} catch(std::exception const & e) {
	std::cerr << e.what() << '\n';
	return -1;
}
