#define BOARD_FRAG \
"#version 430 core\n" \
"\n" \
"in vec2 coord;\n" \
"layout (location = 1) uniform vec3 uWhiteColor;\n" \
"layout (location = 2) uniform vec3 uBlackColor;\n" \
"layout (location = 3) uniform bool uBlackSide;\n" \
"out vec4 outColor;\n" \
"\n" \
"void main() {\n" \
"	ivec2 pos = ivec2(8 * coord);\n" \
"	bool black = (pos.x + pos.y) % 2 == 0 ^^ uBlackSide;\n" \
"	vec3 col = black ? uBlackColor : uWhiteColor;\n" \
"	outColor = vec4(col, 1);\n" \
"}\n" \
""
