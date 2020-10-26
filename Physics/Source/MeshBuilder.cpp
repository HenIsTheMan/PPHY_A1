#include <GL\glew.h>
#include "LoadOBJ.h"
#include "MeshBuilder.h"

Mesh *MeshBuilder::CreateMesh(Mesh::DRAW_MODE drawMode, std::vector<Vertex> vertex_buffer_data, std::vector<GLuint> index_buffer_data){ //For generating mesh
	Mesh *mesh = new Mesh;
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = drawMode;
	return mesh; //Stores VBO and IBO
}

Mesh *MeshBuilder::GenQuad(Color c, float lengthX, float lengthY){ //Quad mesh generator
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color = c;
	v.normal.Set(0, 0, 1.f);

	float midX = lengthX * 0.5f, midY = lengthY * 0.5f;
	v.pos.Set(-midX, midY, 0);
	v.texCoords.Set(0, 1.f);
	vertex_buffer_data.emplace_back(v);
	v.pos.Set(-midX, -midY, 0);
	v.texCoords.Set(0.f, 0.f);
	vertex_buffer_data.emplace_back(v);
	v.pos.Set(midX, midY, 0);
	v.texCoords.Set(1.f, 1.f);
	vertex_buffer_data.emplace_back(v);
	v.pos.Set(midX, -midY, 0);
	v.texCoords.Set(1.f, 0.f);
	vertex_buffer_data.emplace_back(v);

	index_buffer_data.emplace_back(2);
	index_buffer_data.emplace_back(0);
	index_buffer_data.emplace_back(1);
	index_buffer_data.emplace_back(2);
	index_buffer_data.emplace_back(1);
	index_buffer_data.emplace_back(3);
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLES, vertex_buffer_data, index_buffer_data);
}

Mesh *MeshBuilder::GenCircle(Color c, unsigned numSlice, float r){
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color = c;
	v.normal.Set(0.f, 0.f, 1.f);

	v.pos.Set(0.f, 0.f, 0.f);
	v.texCoords.Set(.5f, .5f);
	vertex_buffer_data.emplace_back(v);
	for(unsigned slice = 0; slice < numSlice + 1; ++slice){
		float &&degreePerSlice = -360.f / numSlice, &&theta = slice * degreePerSlice;
		v.pos.Set(r * cos(Math::DegreeToRadian(theta)), r * sin(Math::DegreeToRadian(theta)), 0);
		v.texCoords.Set((v.pos.x / r + 1.f) / 2.f, (v.pos.y / r + 1.f) / 2.f);
		vertex_buffer_data.emplace_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 2; ++slice){
		index_buffer_data.emplace_back(slice + 2);
		index_buffer_data.emplace_back(slice + 1);
		index_buffer_data.emplace_back(0);
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLES, vertex_buffer_data, index_buffer_data);
}

Mesh* MeshBuilder::GenOBJ(const std::string& filePath){
	std::vector<Position> vertices;
	std::vector<TexCoords> UVs;
	std::vector<Vector3> normals;
	if(!LoadOBJ(filePath.c_str(), vertices, UVs, normals)){
		return 0;
	}
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, UVs, normals, index_buffer_data, vertex_buffer_data);
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLES, vertex_buffer_data, index_buffer_data);
}

Mesh* MeshBuilder::GenText(unsigned numRow, unsigned numCol){
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.normal.Set(0, 0, 1.0f);
	vertex_buffer_data.reserve(numRow * numCol * 4);
	index_buffer_data.reserve(numRow * numCol * 6);
	int nCnt = 0;
	float width = 1.f / numCol, height = 1.f / numRow;
	for(unsigned i = 0; i < numRow; ++i){
		for(unsigned j = 0; j < numCol; ++j){
			v.pos.Set(0, 1, 0);
			v.texCoords.Set(width * j, height * (numRow - i)); //0, height
			vertex_buffer_data.emplace_back(v);

			v.pos.Set(0, 0, 0);
			v.texCoords.Set(width * j, height * (numRow - i - 1)); //0, 0
			vertex_buffer_data.emplace_back(v);

			v.pos.Set(1, 1, 0);
			v.texCoords.Set(width * (j + 1), height * (numRow - i)); //width, height
			vertex_buffer_data.emplace_back(v);

			v.pos.Set(1, 0, 0);
			v.texCoords.Set(width * (j + 1), height * (numRow - i - 1)); //width, 0
			vertex_buffer_data.emplace_back(v);

			index_buffer_data.emplace_back(nCnt);
			index_buffer_data.emplace_back(nCnt + 1);
			index_buffer_data.emplace_back(nCnt + 2);
			index_buffer_data.emplace_back(nCnt + 2);
			index_buffer_data.emplace_back(nCnt + 1);
			index_buffer_data.emplace_back(nCnt + 3);
			nCnt = nCnt + 4;
		}
	}
	return CreateMesh(Mesh::DRAW_MODE::DRAW_TRIANGLES, vertex_buffer_data, index_buffer_data);
}