#include "Mesh.h"

MKEngine::mesh::mesh(std::vector <vertex>& vertices, std::vector <GLuint>& indices)
{
	mesh::vertices = vertices;
	mesh::indices = indices;


	VAO.Bind();
	VBO = new MKGraphics::VBO(vertices);
	EBO = new MKGraphics::EBO(indices);
	
	VAO.LinkAttrib(*VBO, 0, 3, GL_FLOAT, sizeof(vertex), (void*)0);
	VAO.LinkAttrib(*VBO, 1, 3, GL_FLOAT, sizeof(vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(*VBO, 2, 3, GL_FLOAT, sizeof(vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(*VBO, 3, 2, GL_FLOAT, sizeof(vertex), (void*)(9 * sizeof(float)));
	VAO.LinkAttrib(*VBO, 4, 3, GL_FLOAT, sizeof(vertex), (void*)(11 * sizeof(float)));
	VAO.LinkAttrib(*VBO, 5, 3, GL_FLOAT, sizeof(vertex), (void*)(14 * sizeof(float)));
	VAO.Unbind();
	VBO->Unbind();
	EBO->Unbind();
}

MKEngine::mesh::mesh()
{
	mesh::vertices = std::vector<vertex>();
	mesh::indices = std::vector<GLuint>();
	VAO.Bind();
	VBO = new MKGraphics::VBO(vertices);
	EBO = new MKGraphics::EBO(indices);

	VAO.LinkAttrib(*VBO, 0, 3, GL_FLOAT, sizeof(vertex), (void*)0);
	VAO.LinkAttrib(*VBO, 1, 3, GL_FLOAT, sizeof(vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(*VBO, 2, 3, GL_FLOAT, sizeof(vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(*VBO, 3, 2, GL_FLOAT, sizeof(vertex), (void*)(9 * sizeof(float)));
	VAO.LinkAttrib(*VBO, 4, 3, GL_FLOAT, sizeof(vertex), (void*)(11 * sizeof(float)));
	VAO.LinkAttrib(*VBO, 5, 3, GL_FLOAT, sizeof(vertex), (void*)(14 * sizeof(float)));

	VAO.Unbind();
	VBO->Unbind();
	EBO->Unbind();
}

void MKEngine::mesh::draw()
{
	VAO.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void MKEngine::mesh::applyData()
{
	VBO->Bind();
	VBO->UpdateDataBuffer(vertices);
	VBO->Unbind();

	EBO->Bind();
	EBO->UpdateDataBuffer(indices);
	VBO->Unbind();
}

void MKEngine::mesh::destroy()
{
	VBO->Delete();
	EBO->Delete();
	VAO.Delete();
}
