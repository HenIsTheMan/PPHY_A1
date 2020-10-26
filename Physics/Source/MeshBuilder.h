#pragma once
#include "Mesh.h"
#include "Vertex.h"

struct MeshBuilder final{
	static Mesh* CreateMesh(Mesh::DRAW_MODE, std::vector<Vertex>, std::vector<GLuint>);
	static Mesh* GenQuad(Color, float, float);
	static Mesh* GenCircle(Color, unsigned, float);
	static Mesh* GenOBJ(const std::string&);
	static Mesh* GenText(unsigned, unsigned);
};