#include "blockMapComponent.h"
#include "graphicsUtils.h"

void MKGame::blockMapComponent::setTile(glm::ivec3 pos, int tileID) {
	auto _chunk = getChunkOrCreateAt(pos);
	if (_chunk == nullptr) {
		_chunk = new chunk();
	}

	int cx = (pos.x < 0 ? -pos.x - 1 : pos.x) % CHUNK_W;
	int cz = (pos.z < 0 ? -pos.z - 1 : pos.z) % CHUNK_D;
	if (pos.x < 0)
		cx = CHUNK_W - 1 - cx;
	if (pos.z < 0)
		cz = CHUNK_D - 1 - cz;
	//LOG::info("({};0;{}) -> ({};{};{})",_chunk->position.x,_chunk->position.z, cx,pos.y,cz );

	_chunk->setTile(glm::ivec3(cx,pos.y,cz), tileID);
	_chunk->isDirty = true;
}

int MKGame::blockMapComponent::getTile(glm::ivec3 pos)
{
	auto _chunk = getChunkOrCreateAt(pos, false);

	if (_chunk == nullptr)
		return 0;

	int cx = (pos.x < 0 ? -pos.x - 1 : pos.x) % CHUNK_W;
	int cz = (pos.z < 0 ? -pos.z - 1 : pos.z) % CHUNK_D;
	if (pos.x < 0)
		cx = CHUNK_W - 1 - cx;
	if (pos.z < 0)
		cz = CHUNK_D - 1 - cz;

	return _chunk->getTile(glm::ivec3(cx,pos.y,cz));
}



void MKGame::blockMapComponent::update() {

	std::map<unsigned int, chunk*>::iterator it;
	for (it = chunksCache.begin(); it != chunksCache.end(); it++)
	{
		it->second->update();
	}
	

}

void MKGame::blockMapComponent::render() {
	std::map<unsigned int, MKGame::chunk*>::iterator it;

	glm::ivec4 bounds = glm::ivec4(drawCenter.x-drawOffset, drawCenter.z-drawOffset, drawCenter.x + drawOffset, drawCenter.z + drawOffset);

	for (it = chunksCache.begin(); it != chunksCache.end(); it++)
	{
		auto chunk = it->second;

		if (chunk->gridPos.x >= bounds.x && chunk->gridPos.x <= bounds.z && chunk->gridPos.y >= bounds.y && chunk->gridPos.y <= bounds.w)
		{
			it->second->draw();
		}
	}

#if (1)

	for (it = chunksCache.begin(); it != chunksCache.end(); it++)
	{
		auto chunk = it->second;

		if (chunk->gridPos.x >= bounds.x && chunk->gridPos.x <= bounds.z && chunk->gridPos.y >= bounds.y && chunk->gridPos.y <= bounds.w)
		{
			glm::vec3 min = chunk->position;
			glm::vec3 max = chunk->position + glm::vec3(CHUNK_W, CHUNK_H, CHUNK_D);
			
/*
#if DEBUG_DRAW
			MKEngine::Utils::drawLine(min, glm::vec3(max.x, min.y, min.z), glm::vec4(0, 1, 0, 1));
			MKEngine::Utils::drawLine(min, glm::vec3(min.x, max.y, min.z), glm::vec4(0, 1, 0, 1));
			MKEngine::Utils::drawLine(min, glm::vec3(max.x, min.y, max.z), glm::vec4(0, 1, 0, 1));
#endif
*/
		}
	}

#endif
}

MKGame::chunk* MKGame::blockMapComponent::getChunkOrCreateAt(glm::ivec3 pos, bool create)
{
	int x = (pos.x<0 ? (pos.x+1- CHUNK_W): pos.x) / CHUNK_W; 
	int z = (pos.z < 0 ? (pos.z + 1 - CHUNK_D) : pos.z) / CHUNK_D;
	unsigned int key = (unsigned int)((z << 16) | (x & 0x0000FFFF));

	std::map<unsigned int, MKGame::chunk*>::iterator it = chunksCache.find(key);

	chunk* _chunk = nullptr;

	if (it == chunksCache.end()) {
		if (create) {
			_chunk = new chunk();
			//LOG::info("{}, {}", x, z);
			_chunk->position = glm::vec3(x * CHUNK_W, 0, z * CHUNK_D);
			_chunk->gridPos = glm::ivec2(x,z);
			chunksCache.insert({ key, _chunk });
		}
	}
	else {
		_chunk = it->second;
	}

	return _chunk;
}
