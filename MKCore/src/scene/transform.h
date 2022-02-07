#pragma once
#include <vector>
#include "../API.h"
#include "glm/glm.hpp"

namespace MKEngine {
	class transform;

	class MK_ENGINE_API transform {
	public:
		glm::vec3 pos = { 0.0f,0.0f,0.0f };
		glm::vec3 eulerRot = { 0.0f,0.0f,0.0f };
		glm::vec3 scale = { 1.0f,1.0f,1.0f };

		glm::mat4 modelMatrix = glm::mat4(1.0);

		MKEngine::transform* parent;
		std::vector<MKEngine::transform*> children;

		void updateLocalMatrix();
		glm::mat4 getGlobalMatrix();
		glm::mat4 getLocalMatrix();
		MKEngine::transform* getRoot();
	private:
		
	};
}