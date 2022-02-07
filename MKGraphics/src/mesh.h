#pragma once

#include "API.H"
#include<string>
#include"VAO.h"
#include"EBO.h"

namespace MKEngine {
	class MK_ENGINE_API mesh
	{
	public:
		std::vector <vertex> vertices;
		std::vector <GLuint> indices;

		MKGraphics::VAO VAO;
		MKGraphics::VBO* VBO;
		MKGraphics::EBO* EBO;

		mesh(std::vector <vertex>& vertices, std::vector <GLuint>& indices);
		mesh();
		void draw();
		void applyData();

		void destroy();
	};
}