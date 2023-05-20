#version 430 core

layout (location = 0) in uvec2 inVert;
layout (location = 0) uniform mat4 uTransformation; // see gfx' quad shader
// layout (location = 1) uniform float uAlphaThreshold; // see gfx' sprite shader
layout (location = 2) uniform vec2 uCustomPosition;
layout (location = 3) uniform vec2[14] uPieceToTex; // king, queen, bishop, knight, rook, pawn, color : bottom-left, size
out vec2 coord; // see gfx' sprite shader

vec2 corner = vec2(inVert.x >> 7, (inVert.x >> 6) & 1);

vec2 relativePos() {
	if((inVert.y >> 7) != 0)
		return uCustomPosition + (corner - 0.5) / 8;
	uint rank = inVert.x >> 3 & (1 << 3) - 1;
	uint file = inVert.x      & (1 << 3) - 1;
	return (vec2(file, rank) + corner) / 8 - 0.5;
}

vec2 texCoords() {
	// TODO fix normalization
	uint color = (inVert.y >> 6) & 1;
	vec2 colorOffset = uPieceToTex[12 + color];
	uint piece = findLSB(inVert.y & (1 << 6) - 1);
	return (colorOffset + uPieceToTex[2 * piece] + corner * uPieceToTex[2 * piece + 1]) / vec2(1854, 618);
}

void main() {
	gl_Position = uTransformation * vec4(relativePos(), 0, 1);
	coord = texCoords();
}
