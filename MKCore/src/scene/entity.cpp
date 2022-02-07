#include "entity.h"

void MKEngine::entity::update()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->update();
	}
}

void MKEngine::entity::render()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->render();
	}
}
