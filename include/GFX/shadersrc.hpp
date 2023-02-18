#define CIRCLE_FRAG \
"#version 430 core\n" \
"\n" \
"in vec2 coord;\n" \
"layout (location = 1) uniform vec4 uColor;\n" \
"out vec4 outColor;\n" \
"\n" \
"void main() {\n" \
"	float x = coord.x;\n" \
"	float y = coord.y;\n" \
"	if(x * x + y * y > 0.25)\n" \
"		discard;\n" \
"	outColor = uColor;\n" \
"}\n" \
""
#define QUAD_VERT \
"#version 430 core\n" \
"\n" \
"layout (location = 0) in vec2 inPos;\n" \
"layout (location = 1) in vec2 inCoord;\n" \
"layout (location = 0) uniform mat4 uTransformation;\n" \
"out vec2 coord;\n" \
"\n" \
"void main() {\n" \
"	gl_Position = uTransformation * vec4(inPos, 0, 1);\n" \
"	coord = inCoord;\n" \
"}\n" \
""
#define RECTANGLE_FRAG \
"#version 430 core\n" \
"\n" \
"layout (location = 1) uniform vec4 uColor;\n" \
"out vec4 outColor;\n" \
"\n" \
"void main() {\n" \
"	outColor = uColor;\n" \
"}\n" \
""
#define SPRITE_FRAG \
"#version 430 core\n" \
"\n" \
"in vec2 coord;\n" \
"uniform sampler2D uTex; // implictly texture unit 0 (binding = 0)\n" \
"layout (location = 1) uniform float uAlphaThreshold;\n" \
"out vec4 outColor;\n" \
"\n" \
"void main() {\n" \
"	outColor = texture(uTex, coord);\n" \
"	if(outColor.a <= uAlphaThreshold)\n" \
"		discard;\n" \
"}\n" \
""
#define SPRITESHEET_FRAG \
"#version 430 core\n" \
"\n" \
"in vec2 coord;\n" \
"uniform sampler2D uTex; // implictly texture unit 0 (binding = 0)\n" \
"layout (location = 1) uniform float uAlphaThreshold;\n" \
"layout (location = 2) uniform vec2 uPosition; // bottom left\n" \
"layout (location = 3) uniform vec2 uRelativeSize = vec2(1, 1);\n" \
"out vec4 outColor;\n" \
"\n" \
"void main() {\n" \
"	vec2 texCoord = uPosition + coord * uRelativeSize;\n" \
"	outColor = texture(uTex, texCoord);\n" \
"	if(outColor.a <= uAlphaThreshold)\n" \
"		discard;\n" \
"}\n" \
""
