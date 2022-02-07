#pragma once
#include "API.h"
#include "MKGraphics.h"
#include "mesh.h"
#include "glm/glm.hpp"

namespace MKEngine {
	namespace Utils {

		MK_ENGINE_API MKEngine::mesh* makeQuad(glm::vec2 min, glm::vec2 max, MKGraphics::Axis axis);
		MK_ENGINE_API MKEngine::mesh* makeCube();
	}
}