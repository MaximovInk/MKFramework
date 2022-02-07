#pragma once

#include "vertex.h"
#include<glad/gl.h>
#include<vector>

namespace MKGraphics {
	class VBO
	{
	public:
		GLuint ID;
		VBO(std::vector<vertex> vertices);

		void Bind();
		void Unbind();
		void UpdateDataBuffer(std::vector<vertex> vertices);
		void Delete();
	};
}