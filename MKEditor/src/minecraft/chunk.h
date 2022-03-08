#pragma once
#include "mesh.h"
#include "shader.h"

#include <stdint.h>
#include <vector>
#include "tile.h"

namespace MKGame {

#define CHUNK_W 128 //x
#define CHUNK_H 11 //y
#define CHUNK_D 128//z
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

	class chunk {
	public:
		MKEngine::mesh* _mesh;

		uint8_t* data;
		uint8_t* bitmasks;

		std::vector<tile> tiles;

		bool isDirty;

		chunk();

		void updateBitmasks();
		void updateBitmask(int x, int y, int z);
		void generateMesh();
		void draw();

		void setTile(glm::ivec3 pos, int tileID);
		int getTile(glm::ivec3 pos);
		void checkTile(int index);
	private:
			void emitFace(glm::vec3 pos, CUBE_FACE face, glm::vec2 uvOffset, glm::vec2 uvUnit);
	};
}