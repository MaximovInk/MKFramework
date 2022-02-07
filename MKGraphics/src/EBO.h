#pragma once

#include<glad/gl.h>
#include <vector>

namespace MKGraphics {
	class EBO
	{
	public:
		GLuint ID;
		EBO(std::vector<GLuint>& indices);

		void Bind();
		void Unbind();
		void Delete();
		void UpdateDataBuffer(std::vector<GLuint> indices);

	};
}