#pragma once
#include "Vertex.h"

struct Mesh final{
	enum class DRAW_MODE{
		DRAW_TRIANGLES, DRAW_TRIANGLE_STRIP, DRAW_LINES, DRAW_FAN, DRAW_MODE_LAST
	};
	Mesh(), ~Mesh();
	void Render() const;
	void Render(unsigned, unsigned) const;
	DRAW_MODE mode;
	unsigned vertexBuffer, colorBuffer, indexBuffer, textureID;
};