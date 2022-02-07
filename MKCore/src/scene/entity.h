#pragma once
#include <vector>
#include <string>
#include "transform.h"
#include "component.h"
#include "../API.h"

namespace MKEngine {

	class component;
	class scene;

	class MK_ENGINE_API entity {
	public:
		MKEngine::scene* scene;
		std::string name;

		MKEngine::transform* transform;

		std::vector<MKEngine::component*> components;

		void update();
		void render();

		entity()
		{
			this->transform = new MKEngine::transform;
		}
		~entity() {
			delete this->transform;
		}
	};
}