#pragma once

#include "tile.h"
#include "glm/glm.hpp"

#include "texture.h"

namespace MKGame {

#define TILES_H 3.0f
#define TILES_W 2.0f
#define TILE_UNIT_X 1.0f/TILES_W
#define TILE_UNIT_Y 1.0f/TILES_H
#define TILE_UNIT glm::vec2(TILE_UNIT_X,TILE_UNIT_Y)
	
	class block;

	static class blocks {

	public:
		static texture* getTex();
		static void setTex(texture* _tex);
		
		static MKGame::block* getData(int id);

		static void registerData(MKGame::block* block);

		static glm::vec2 getUVfromTilePos(glm::ivec2 pos);
	};

	class block {
	public:
		int id;
		virtual glm::vec2 getTextureOffset(MKGame::CUBE_FACE face);
	};
	class dirt : public block {
	public:
		 glm::vec2 getTextureOffset(MKGame::CUBE_FACE face) ;
	};

	class grass : public block {
	public:
		 glm::vec2 getTextureOffset(MKGame::CUBE_FACE face);
	};
}