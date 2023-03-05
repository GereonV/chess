#version 430 core

in vec2 coord; // see gfx' quad shader
layout (location = 1) uniform vec3 uLightColor;
layout (location = 2) uniform vec3 uDarkColor;
layout (location = 3) uniform bool uFlipped;
out vec4 outColor;

void main() {
	ivec2 pos = ivec2(8 * coord);
	bool black = (pos.x + pos.y) % 2 == 0 ^^ uFlipped;
	vec3 col = black ? uDarkColor : uLightColor;
	outColor = vec4(col, 1);
}
