#pragma once
#include "API.h"
#include "MKGraphics.h"
#include "mesh.h"
#include "glm/glm.hpp"

namespace MKEngine {
	namespace Utils {

		MK_ENGINE_API MKEngine::mesh* makeQuad(glm::vec2 min, glm::vec2 max, MKGraphics::Axis axis);
		MK_ENGINE_API MKEngine::mesh* makeCube();

		MK_ENGINE_API void setLineWidth(float width);

		MK_ENGINE_API void setCamMatrix(glm::mat4 matrix);

		MK_ENGINE_API void drawLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color);
		MK_ENGINE_API void drawLine(glm::vec3 pos1, glm::vec3 pos2, texture texure);

		MK_ENGINE_API void drawQuad(glm::vec3 pos, glm::vec3 rot, glm::vec2 size, glm::vec4 color);
		MK_ENGINE_API void drawQuad(glm::vec3 pos, glm::vec4 size, texture texure);

		MK_ENGINE_API void beginRender();
		MK_ENGINE_API void endRender();
	}
}