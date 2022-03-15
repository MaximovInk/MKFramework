#pragma once

#include "string"
#include<glad/gl.h>
#include "API.H"
//#include "stb/stb_image.h"

class MK_ENGINE_API texture
{
public:
	texture(const char* image);

	texture(int width, int height, const void* pixels, GLenum format, GLenum pixelType);

	texture(GLuint ID, const char* type, std::string path);
	texture(GLuint ID, const char* type, GLenum slot, std::string path);

	texture(const char* image, const char* texType, GLenum slot, GLenum pixelType);

	texture(const char* image, const char* texType, GLenum format, GLenum slot, GLenum pixelType);

	//void texUnit(shader& shader, const char* uniform, GLuint unit);
	void bind();
	void unbind();
	void destroy();

	void setType(const char* type);
	void setSlot(GLenum slot);
	void setFilter(GLint mag, GLint min);
	void setWrap(GLint s, GLint t);
	void generateMipmap();
	

private:
	GLuint ID;
	const char* type;
	std::string path;
	GLenum glTexType;
};