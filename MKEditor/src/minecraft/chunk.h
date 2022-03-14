#pragma once
#include "mesh.h"
#include "shader.h"

#include <stdint.h>
#include <vector>
#include "tile.h"

namespace MKGame {

#define CHUNK_W 16 //x
#define CHUNK_H 20 //y
#define CHUNK_D 16//z
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

	class chunk {
	public:
		MKEngine::mesh* _mesh;

		uint8_t* data;
		uint8_t* bitmasks;

		std::vector<tile> tiles;

		bool isDirty;

		glm::vec3 position;
		glm::ivec2 gridPos;

		chunk();

		void updateBitmasks();
		void updateBitmask(int x, int y, int z);
		void generateMesh();
		void generatePhysicsMesh();
		void draw();
		void update();

		void setTile(glm::ivec3 pos, int tileID);
		int getTile(glm::ivec3 pos);
		//void checkTile(int index);
	private:
			void emitFace(glm::vec3 pos, CUBE_FACE face, glm::vec2 uvOffset, glm::vec2 uvUnit);
	};
}