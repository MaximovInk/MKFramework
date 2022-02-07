#pragma once

#include<glad/gl.h>
#include"VBO.h"

namespace MKGraphics {
	class VAO
	{
	public:
		GLuint ID;
		VAO();

		void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		void Bind();
		void Unbind();
		void Delete();
	};
}