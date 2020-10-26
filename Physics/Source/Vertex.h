#pragma once
#include "Vector3.h"

struct Position final{
	float x, y, z;
	Position(float x = 0, float y = 0, float z = 0){
		Set(x, y, z);
	}
	void Set(float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
};

struct Color final{
	float r, g, b;
	Color(float r = 1, float g = 1, float b = 1){
		Set(r, g, b);
	}
	void Set(float r, float g, float b) {this->r = r; this->g = g; this->b = b;}
};

struct TexCoords final{
	float u, v;
	TexCoords(float u = 0, float v = 0){
		Set(u, v);
	}
	void Set(float u, float v) {this->u = u; this->v = v;}
};

struct Vertex final{
	Position pos;
	Color color;
	Vector3 normal;
	TexCoords texCoords;
	Vertex() = default;
};