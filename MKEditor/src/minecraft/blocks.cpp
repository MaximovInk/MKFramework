#include "blocks.h"
#include "MKUtils.h"
#include <vector>

texture* _tex;
std::vector<MKGame::block*> _blocks;

glm::vec2 MKGame::dirt::getTextureOffset(MKGame::CUBE_FACE face)
{
	//LOG::info("dirt");
	return MKGame::blocks::getUVfromTilePos(glm::ivec2(0,2));
}

glm::vec2 MKGame::grass::getTextureOffset(MKGame::CUBE_FACE face)
{
	//LOG::info("grass");
	if (face == MKGame::CUBE_FACE::TOP)
		return MKGame::blocks::getUVfromTilePos(glm::ivec2(0,0));
	return MKGame::blocks::getUVfromTilePos(glm::ivec2(1,2));
}

texture* MKGame::blocks::getTex()
{
	return _tex;
}

void MKGame::blocks::setTex(texture* tex)
{
	_tex = tex;
}

MKGame::block* MKGame::blocks::getData(int id)
{
	return _blocks[id-1];
}

void MKGame::blocks::registerData(MKGame::block* block)
{
	block->id = _blocks.size();
	_blocks.push_back(block);
}

glm::vec2 MKGame::blocks::getUVfromTilePos(glm::ivec2 pos)
{
	return glm::vec2(1.0f/TILES_W*pos.x,1.0f/TILES_H*pos.y);
}

glm::vec2 MKGame::block::getTextureOffset(MKGame::CUBE_FACE face)
{
	LOG::info("base");
	return glm::vec2(0.0f);
}
