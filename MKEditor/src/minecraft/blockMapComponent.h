#pragma once
#include "glm/gtx/string_cast.hpp"
#include "MKUtils.h"
#include "scene/component.h"
#include "chunk.h"
#include "glm/glm.hpp"

namespace MKGame {

	class blockMapComponent : public MKEngine::component {
	public:
		using component::component;

		bool checkAABB(glm::vec3 min, glm::vec3 max) {
			//LOG::info("{};{}",glm::to_string(min),glm::to_string(max));
			
			for (int ix = std::floor(min.x); ix <= std::ceil(max.x); ix++)
			{
				for (int iy = std::floor(min.y); iy <= std::ceil(max.y); iy++)
				{
					for (int iz = std::floor(min.z); iz <= std::ceil(max.z); iz++)
					{
						//LOG::info("{};{};{}", int(ix),int(iy),int(iz));
						if (_chunk->getTile(glm::ivec3(ix, iy, iz)) != 0) {
							//LOG::info("{};{};{}={}", ix, iy, iz, _chunk->getTile(glm::ivec3(ix, iy, iz)) );
							return true;
						}
					}
				}
			}
			

			return false;
		}

		void setTile(glm::ivec3 pos, int tileID) {
			if (_chunk == nullptr) {
				_chunk = new chunk();
			}
			_chunk->setTile(pos, tileID);
			_chunk->isDirty = true;
		}

		virtual void update() {
			if (_chunk->isDirty) {
				_chunk->updateBitmasks();
				_chunk->generateMesh();
				_chunk->isDirty = false;
			}
		}

		virtual void render() {
			_chunk->draw();
		};
	private:
		chunk* _chunk;

		/*
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
		*/
	};

}