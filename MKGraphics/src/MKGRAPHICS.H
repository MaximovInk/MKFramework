#pragma once
#include "API.h"

#include "glad/gl.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "cubeMap.h"

namespace MKGraphics {
	enum Axis {
		AXIS_X,AXIS_Y,AXIS_Z
	};

	MK_ENGINE_API void loadGL(GLADloadfunc func);
	MK_ENGINE_API void clear(GLbitfield mask);
	MK_ENGINE_API void clearColor(GLfloat r,GLfloat g,GLfloat b,GLfloat a);
	MK_ENGINE_API void setViewport(GLint x, GLint y, GLsizei width, GLsizei height);
	MK_ENGINE_API void enable(GLenum cap);
	MK_ENGINE_API void blendEnable();
	MK_ENGINE_API void disable(GLenum cap);
	MK_ENGINE_API void setPolygonMode(GLenum face, GLenum mode);
	
}