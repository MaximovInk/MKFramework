#include "chunk.h"
#include <vector>
#include "MKUtils.h"
#include <glm/gtx/string_cast.hpp>
#include "blocks.h"

using namespace MKGame;

static const uint8_t UNIQUE_INDICES[] = { 0, 1, 2, 5 };

static const uint8_t FACE_INDICES[] = { 0, 1, 2, 0, 2, 3 };

static const uint8_t CUBE_INDICES[] = {
	4, 7, 6, 4, 6, 5, // (south (+z))
	3, 0, 1, 3, 1, 2, // (north (-z))
	7, 3, 2, 7, 2, 6, // (east  (+x))
	0, 4, 5, 0, 5, 1, // (west  (-x))
	2, 1, 5, 2, 5, 6, // (up    (+y))
	0, 3, 7, 0, 7, 4  // (down  (-y))
};

static const glm::vec3 CUBE_VERTICES[] = {
	glm::vec3(0, 0, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(1, 1, 0),
	glm::vec3(1, 0, 0),
	glm::vec3(0, 0, 1),
	glm::vec3(0, 1, 1),
	glm::vec3(1, 1, 1),
	glm::vec3(1, 0, 1)
};

static const glm::vec3 CUBE_NORMALS[] = {
	glm::vec3(0,  0,  1),
	glm::vec3(0,  0, -1),
	glm::vec3(1,  0,  0),
	glm::vec3(-1,  0,  0),
	glm::vec3(0,  1,  0),
	glm::vec3(0, -1,  0),
};
static const glm::vec2 CUBE_UVS[] = {
	glm::vec2(0, 0),
	glm::vec2(1, 0),
	glm::vec2(1, 1),
	glm::vec2(0, 1),
};

void MKGame::chunk::emitFace(glm::vec3 pos, CUBE_FACE face, glm::vec2 uvOffset, glm::vec2 uvUnit) {

	int offset = this->_mesh->vertices.size();

	for (int i = 0; i < 4; i++) {

		vertex v;
		v.position = pos + CUBE_VERTICES[CUBE_INDICES[(face * 6) + UNIQUE_INDICES[i]]];
		v.texUV = (CUBE_UVS[i] * uvUnit) + uvOffset;
		//LOG::info(" {} {} {}", i, glm::to_string(v.position), glm::to_string(v.texUV));
		_mesh->vertices.push_back(v);
	}

	for (size_t i = 0; i < 6; i++)
	{
		_mesh->indices.push_back(offset+FACE_INDICES[i]);
	}
}

MKGame::chunk::chunk()
{
	_mesh = new MKEngine::mesh();
	data = new uint8_t[CHUNK_VOL]();
	bitmasks = new uint8_t[CHUNK_VOL];
}

void MKGame::chunk::updateBitmasks()
{
	for (int x = 0; x < CHUNK_W; x++)
	{
		for (int y = 0; y < CHUNK_H; y++)
		{
			for (int z = 0; z < CHUNK_D; z++)
			{
				updateBitmask(x, y, z);
			}
		}
	}

}

enum CUBE_FACE_MASKS {
	SOUTH =		0b0000'0001 ,         // +z
	NORTH =		0b0000'0010,         // -z
	EAST =		0b0000'0100,         // +x
	WEST =		0b0000'1000,         // -x
	TOP =		0b0001'0000,           // +y
	BOTTOM =	0b0010'0000,         // -y
};


void MKGame::chunk::updateBitmask(int x, int y, int z)
{
	uint8_t bitmask = 0;

	if (z<CHUNK_D && getTile(glm::ivec3(x, y, z+1)) != 0)
		bitmask |= CUBE_FACE_MASKS::SOUTH;
	if (z > 0 && getTile(glm::ivec3(x, y, z -1)) != 0)
		bitmask |= CUBE_FACE_MASKS::NORTH;
	if (x < CHUNK_W && getTile(glm::ivec3(x+1, y, z )) != 0)
		bitmask |= CUBE_FACE_MASKS::EAST;
	if (x > 0 && getTile(glm::ivec3(x-1, y, z )) != 0)
		bitmask |= CUBE_FACE_MASKS::WEST;
	if (y < CHUNK_H && getTile(glm::ivec3(x, y+1, z )) != 0)
		bitmask |= CUBE_FACE_MASKS::TOP;
	if (y > 0 && getTile(glm::ivec3(x, y-1, z )) != 0)
		bitmask |= CUBE_FACE_MASKS::BOTTOM;

	int index = y * CHUNK_W * CHUNK_D + z * CHUNK_W + x;
	bitmasks[index] = bitmask;
}

void MKGame::chunk::generateMesh()
{

	for (int x = 0; x < CHUNK_W; x++)
	{
		for (int y = 0; y < CHUNK_H; y++)
		{
			for (int z = 0; z < CHUNK_D; z++)
			{
				glm::vec3 pos = glm::vec3(x, y, z);

				int index = pos.y * CHUNK_W * CHUNK_D + pos.z * CHUNK_W + pos.x;
				int tileID = data[index];
				if (tileID == 0)
					continue;
				else {
					//LOG::info("GEN");
				}

				auto data = MKGame::blocks::getData(tileID);
				auto bitmask = bitmasks[index];

				if ((bitmask & CUBE_FACE_MASKS::NORTH) == 0) 
					emitFace(pos, MKGame::NORTH, data->getTextureOffset(MKGame::NORTH), TILE_UNIT);
				if ((bitmask & CUBE_FACE_MASKS::SOUTH) == 0)
					emitFace(pos, MKGame::SOUTH, data->getTextureOffset(MKGame::SOUTH), TILE_UNIT);
				if ((bitmask & CUBE_FACE_MASKS::WEST) == 0)
					emitFace(pos, MKGame::WEST, data->getTextureOffset(MKGame::WEST), TILE_UNIT);
				if ((bitmask & CUBE_FACE_MASKS::EAST) == 0)
					emitFace(pos, MKGame::EAST, data->getTextureOffset(MKGame::EAST), TILE_UNIT);
				if ((bitmask & CUBE_FACE_MASKS::TOP) == 0)
					emitFace(pos, MKGame::TOP, data->getTextureOffset(MKGame::TOP), TILE_UNIT);
				if ((bitmask & CUBE_FACE_MASKS::BOTTOM) == 0)
					emitFace(pos, MKGame::BOTTOM, data->getTextureOffset(MKGame::BOTTOM), TILE_UNIT);
				
				
			}
		}
	}

	/*
	glm::vec3 pos = glm::vec3(1.0);

	emitFace(pos, MKGame::TOP, glm::vec2(0.0), glm::vec2(1.0));
	emitFace(pos, MKGame::NORTH, glm::vec2(0.0), glm::vec2(1.0));
	emitFace(pos, MKGame::SOUTH, glm::vec2(0.0), glm::vec2(1.0));
	emitFace(pos, MKGame::BOTTOM, glm::vec2(0.0), glm::vec2(1.0));
	emitFace(pos, MKGame::WEST, glm::vec2(0.0), glm::vec2(1.0));
	emitFace(pos, MKGame::EAST, glm::vec2(0.0), glm::vec2(1.0));
	*/

	_mesh->applyData();
	//emitFace(glm::vec3(2,0,0), MKGame::NORTH, glm::vec2(0.0), glm::vec2(1.0));
}

void MKGame::chunk::draw()
{
	MKGame::blocks::getTex()->bind();
	this->_mesh->draw();
}

void MKGame::chunk::setTile(glm::ivec3 pos, int tileID)
{
	int index = pos.y * CHUNK_W * CHUNK_D + pos.z * CHUNK_W + pos.x;
	//LOG::info("s-POS:{} \t| IND:{} \t| VAL:{}", glm::to_string(pos), std::to_string(index), tileID);
	data[index] = tileID;
}

int MKGame::chunk::getTile(glm::ivec3 pos)
{
	int index = pos.y * CHUNK_W * CHUNK_D + pos.z * CHUNK_W + pos.x;

	return data[index];
}

void MKGame::chunk::checkTile(int index)
{
	int y = index / (CHUNK_W * CHUNK_D);
	int dV = index % (CHUNK_W * CHUNK_D);
	int z = dV / CHUNK_W;
	int x = dV % CHUNK_W;

	glm::ivec3 pos = glm::ivec3(x,y,z);
	int tileID = data[index];

	LOG::info("g-POS:{} \t| IND:{} \t| VAL:{}", glm::to_string(pos), std::to_string(index), tileID);
}
