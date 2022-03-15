#pragma once

#include "glm/glm.hpp"
#include "../minecraft/blockMapComponent.h"



namespace MKGame {

	class Hitbox {
	public:
		glm::vec3 pos;
		glm::vec3 halfSize;
		glm::vec3 velocity;
		float linear_damp;
		bool isGrounded;

		Hitbox(glm::vec3 pos, glm::vec3 halfSize)
			: pos(pos), halfSize(halfSize), velocity(0.0f, 0.0f, 0.0f), linear_damp(0.1f), isGrounded(false)
		{

		}
	};

	bool isBlockObstacle(blockMapComponent* blockMap, int x, int y, int z);

	void physStep(float deltaTime, unsigned physSubsteps, blockMapComponent* blockMap, MKGame::Hitbox* hitbox);

	bool isBlockInside(int x, int y, int z, MKGame::Hitbox* hitbox);

}