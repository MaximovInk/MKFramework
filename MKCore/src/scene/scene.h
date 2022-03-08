#pragma once
#include <vector>
#include "entity.h"
#include "component.h"
#include "camera.h"
#include "../API.h"

namespace MKEngine {
	class MK_ENGINE_API scene {
	public:
		void update();
		void render();

		camera* cam;

		MKEngine::entity* createEntity();
		MKEngine::entity* createEntity(std::string name);
		void deleteEntity(MKEngine::entity* value);
		std::vector<MKEngine::entity*> entities;
	};
}