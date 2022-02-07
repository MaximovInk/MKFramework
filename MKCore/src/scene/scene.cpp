#include "scene.h"
#include "../MKCore.h"
#include "MKUtils.h"

void MKEngine::scene::update()
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->update();
	}
}

void MKEngine::scene::render()
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->render();
	}
}

MKEngine::entity* MKEngine::scene::createEntity()
{
	auto ent = new entity();
	ent->name = "new entity";
	ent->scene = this;
	entities.push_back(ent);
	return ent;
}

void MKEngine::scene::deleteEntity(entity* entity)
{
	entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
	delete entity;
}