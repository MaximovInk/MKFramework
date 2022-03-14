#pragma once
#include "map"
#include "glm/gtx/string_cast.hpp"
#include "MKUtils.h"
#include "scene/component.h"
#include "chunk.h"
#include "glm/glm.hpp"

namespace MKGame {

	class blockMapComponent : public MKEngine::component {
	public:
		using component::component;

		void setTile(glm::ivec3 pos, int tileID);
		int getTile(glm::ivec3 pos);

		int drawOffset = 2;
		glm::vec3 drawCenter;

		virtual void update();

		virtual void render();
		//chunk* _chunk;

	private:
		std::map<unsigned int, MKGame::chunk*> chunksCache;

		MKGame::chunk* getChunkOrCreateAt(glm::ivec3 pos,bool create = true);
	};
}