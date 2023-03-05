#version 430 core

layout (location = 0) in uvec2 inVert;
layout (location = 0) uniform mat4 uTransformation; // see gfx' quad shader
// layout (location = 1) uniform float uAlphaThreshold; // see gfx' sprite shader
layout (location = 2) uniform vec2 uCustomPosition;
layout (location = 3) uniform vec2[7][2] uPieceToTex; // king, queen, bishop, knight, rook, pawn, color : bottom-left, size
out vec2 coord; // see gfx' sprite shader

vec2 corner = vec2(inVert.x >> 7, (inVert.x >> 6) & 1);

vec2 relativePos() {
	// return corner;
	if((inVert.y >> 7) != 0)
		return uCustomPosition + (corner - 0.5) / 8;
	uint rank = inVert.x >> 3 & (1 << 3) - 1;
	uint file = inVert.x      & (1 << 3) - 1;
	return (vec2(file, rank) + corner) / 8 - 0.5;
}

vec2 texCoords() {
	vec2 colorOffset = uPieceToTex[6][(inVert.y >> 6) & 1];
	uint piece = findLSB(inVert.y & (1 << 6) - 1);
	return colorOffset + uPieceToTex[piece][0] + corner * uPieceToTex[piece][1];
}

void main() {
	gl_Position = uTransformation * vec4(relativePos(), 0, 1);
	coord = texCoords();
}
