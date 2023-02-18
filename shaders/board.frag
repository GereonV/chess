#version 430 core

in vec2 coord;
layout (location = 1) uniform vec3 uWhiteColor;
layout (location = 2) uniform vec3 uBlackColor;
layout (location = 3) uniform bool uBlackSide;
out vec4 outColor;

void main() {
	ivec2 pos = ivec2(8 * coord);
	bool black = (pos.x + pos.y) % 2 == 0 ^^ uBlackSide;
	vec3 col = black ? uBlackColor : uWhiteColor;
	outColor = vec4(col, 1);
}
