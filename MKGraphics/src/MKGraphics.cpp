#include "MKGraphics.h"
#include <iostream>
#include "glad/gl.h"
#include "MKUtils.h"

#define STB_IMAGE_IMPLEMENTATION    
#include "stb/stb_image.h"

MK_ENGINE_API void MKGraphics::loadGL(GLADloadfunc func)
{
	const auto version = gladLoadGL(reinterpret_cast<GLADloadfunc>(func));
	LOG::info("GL {0}.{1}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
}

MK_ENGINE_API void MKGraphics::clear(GLbitfield mask)
{
	glClear(mask);
}

MK_ENGINE_API void MKGraphics::clearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
}

void MKGraphics::setViewport(GLint x,GLint y,GLsizei width, GLsizei height)
{
	glViewport(x, y, width, height);
}

void MKGraphics::enable(GLenum cap)
{
	glEnable(cap);
}

void MKGraphics::disable(GLenum cap)
{
	glDisable(cap);
}

void MKGraphics::setPolygonMode(GLenum face, GLenum mode)
{
	glPolygonMode(face, mode);
}

