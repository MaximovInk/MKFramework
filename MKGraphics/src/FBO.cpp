#include "FBO.h"

#include "spdlog/spdlog.h"

void checkIfComplete();

FBO::FBO()
{
	glGenFramebuffers(1, &ID);
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

unsigned int FBO::AttachTexture(int width, int height, GLenum internalFormat, GLenum format)
{
	Bind();
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	Unbind();

	return tex;
}

unsigned int FBO::AttachRenderTexture(int width, int height)
{
	Bind();
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	Unbind();

	return rbo;
}

int FBO::getHeight()
{
	return 0;
}

int FBO::getWidth()
{
	return 0;
}

void FBO::CheckIfComplete()
{
	Bind();
	GLenum a = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (a != GL_FRAMEBUFFER_COMPLETE)
		spdlog::error("FRAMEBUFFER:: Framebuffer is not complete! {}", a);
	else
		spdlog::info("FRAMEBUFFER:: Framebuffer complete!");
	Unbind();
}


void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Delete()
{
	glDeleteFramebuffers(1, &ID);
	if (rbo != 0) {
		glDeleteRenderbuffers(1, &rbo);
	}
	if (tex != 0) {
		glDeleteTextures(1, &tex);
	}

}

