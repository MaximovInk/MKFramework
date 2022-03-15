#include "texture.h"
#include "MKUtils.h"
#include "stb/stb_image.h"

texture::texture(const char* image)
{
	glTexType = GL_TEXTURE_2D;
	type = "diffuse";
	GLenum format = GL_RGBA;
	auto slot = 0;
	auto pixelType = GL_UNSIGNED_BYTE;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	if (bytes) {
		if (numColCh == 1)
			format = GL_RED;
		else if (numColCh == 3)
			format = GL_RGB;
		else if (numColCh == 4)
			format = GL_RGBA;
		LOG::info(" {} {} {} {} {} {}", image, widthImg, heightImg, numColCh, format, slot);

		glGenTextures(1, &ID);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(glTexType, ID);

		glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(glTexType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(glTexType, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(glTexType, 0, format, widthImg, heightImg, 0, format, pixelType, bytes);
		glGenerateMipmap(glTexType);
		glBindTexture(glTexType, 0);

		stbi_image_free(bytes);
	}
}

texture::texture(int width, int height, const void* pixels, GLenum format, GLenum pixelType)
{
	glTexType = glTexType;
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(glTexType, ID);

	glTexImage2D(glTexType, 0, format, width, height, 0, format, pixelType, pixels);

	glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(glTexType, 0);
}

texture::texture(GLuint ID, const char* type, std::string path)
{
	this->ID = ID;
	this->type = type;
	this->path = path;
}

texture::texture(GLuint ID, const char* type, GLenum slot, std::string path)
{
	glTexType = GL_TEXTURE_2D;
	this->ID = ID;
	this->type = type;
	this->path = path;
}

texture::texture(const char* image, const char* texType, GLenum slot, GLenum pixelType)
{
	glTexType = GL_TEXTURE_2D;
	type = texType;
	GLenum format = GL_RGBA;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	if (bytes) {
		if (numColCh == 1)
			format = GL_RED;
		else if (numColCh == 3)
			format = GL_RGB;
		else if (numColCh == 4)
			format = GL_RGBA;
		LOG::info(" {} {} {} {} {} {}", image, widthImg, heightImg, numColCh, format, slot);

		glGenTextures(1, &ID);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(glTexType, ID);

		glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(glTexType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(glTexType, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(glTexType, 0, format, widthImg, heightImg, 0, format, pixelType, bytes);
		glGenerateMipmap(glTexType);
		glBindTexture(glTexType, 0);

		stbi_image_free(bytes);
	}
}

texture::texture(const char* image, const char* texType, GLenum format, GLenum slot, GLenum pixelType)
{
	type = texType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	if (bytes) {
		glGenTextures(1, &ID);
		glBindTexture(glTexType, ID);

		glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(glTexType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(glTexType, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(glTexType, 0, format, widthImg, heightImg, 0, format, pixelType, bytes);
		glGenerateMipmap(glTexType);
		glBindTexture(glTexType, 0);


		stbi_image_free(bytes);
	}

}
/*

void texture::texUnit(shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.use();
	glUniform1i(texUni, unit);
}
*/

void texture::bind()
{
	glBindTexture(glTexType, ID);
}

void texture::unbind()
{
	glBindTexture(glTexType, 0);
}

void texture::destroy()
{
	glDeleteTextures(1, &ID);
}

void texture::setType(const char* type)
{
	this->type = type;
}

void texture::setSlot(GLenum slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
}

void texture::setFilter(GLint mag, GLint min)
{
	glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, mag);
}

void texture::setWrap(GLint s, GLint t)
{
	glTexParameteri(glTexType, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(glTexType, GL_TEXTURE_WRAP_T, t);

}

void texture::generateMipmap()
{
	glGenerateMipmap(glTexType);
}
