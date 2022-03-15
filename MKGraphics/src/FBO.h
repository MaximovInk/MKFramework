#pragma once

#include<glad/gl.h>
#include <vector>
#include "API.h"

class MK_ENGINE_API FBO
{
public:
	GLuint ID;
	FBO();

	void Bind();
	unsigned int AttachTexture(int width, int height, GLenum internalFormat = GL_RGB, GLenum format = GL_RGB);
	unsigned int AttachRenderTexture(int width, int height);
	int getHeight();
	int getWidth();
	void CheckIfComplete();
	void Unbind();
	void Delete();

private:
	unsigned int rbo = 0;
	unsigned int tex = 0;
};
