#pragma once
#include "../API.h"
#include "entity.h"

namespace MKEngine {

	class entity;

	class MK_ENGINE_API component {

	public:
		MKEngine::entity* entity;

		component(MKEngine::entity* entity)
		{
			this->entity = entity;
		}

		virtual void update() = 0;
		virtual void render() = 0;
	};
}