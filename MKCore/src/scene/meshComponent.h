#pragma once
#include "component.h"
#include "mesh.h"
#include "shader.h"

#include "../API.h"

namespace MKEngine {
	class MK_ENGINE_API meshComponent : public  MKEngine::component {
	public:
		MKEngine::mesh* mesh;
		shader* shader;

		using component::component;

		virtual void update() {
			
		};
		virtual void render() {
			if (this->mesh != nullptr) {
				this->shader->use();
				this->shader->setMat4("camMatrix", this->entity->scene->cam->Matrix);
				this->shader->setMat4("model", this->entity->transform->getLocalMatrix());
				this->mesh->draw();
			}
		};
	};
}