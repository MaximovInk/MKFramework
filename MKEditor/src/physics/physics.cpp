#include "physics.h"

#define E 0.03
#define DEFAULT_FRICTION 10.0


const glm::vec3 gravity = glm::vec3(0, -19.6f, 0);
float gravityScale = 1.0;

bool MKGame::isBlockObstacle(blockMapComponent* blockMap, int x, int y, int z) {
	return blockMap->getTile(glm::ivec3(x, y, z)) != 0;
}

void MKGame::physStep(float deltaTime, unsigned physSubsteps, blockMapComponent* blockMap, MKGame::Hitbox* hitbox) {
	hitbox->isGrounded = false;
	for (unsigned i = 0; i < physSubsteps; i++)
	{
		float dt = deltaTime / (float)physSubsteps;
		float linearDump = hitbox->linear_damp;
		glm::vec3& pos = hitbox->pos;
		glm::vec3& half = hitbox->halfSize;
		glm::vec3& vel = hitbox->velocity;
		vel.x += gravity.x * dt * gravityScale;
		vel.y += gravity.y * dt * gravityScale;
		vel.z += gravity.z * dt * gravityScale;

		float px = pos.x;
		float pz = pos.z;

		if (vel.x < 0.0) {
			for (int y = floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
				for (int z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
					int x = floor(pos.x - half.x - E);
					if (isBlockObstacle(blockMap, x, y, z)) {
						vel.x *= 0.0;
						pos.x = x + 1 + half.x + E;
						break;
					}
				}
			}
		}
		if (vel.x > 0.0) {
			for (int y = floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
				for (int z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
					int x = floor(pos.x + half.x + E);
					if (isBlockObstacle(blockMap, x, y, z)) {
						vel.x *= 0.0;
						pos.x = x - half.x - E;
						break;
					}
				}
			}
		}

		if (vel.z < 0.0) {
			for (int y = floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
				for (int x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
					int z = floor(pos.z - half.z - E);
					if (isBlockObstacle(blockMap, x, y, z)) {
						vel.z *= 0.0;
						pos.z = z + 1 + half.z + E;
						break;
					}
				}
			}
		}

		if (vel.z > 0.0) {
			for (int y = floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
				for (int x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
					int z = floor(pos.z + half.z + E);
					if (isBlockObstacle(blockMap, x, y, z)) {
						vel.z *= 0.0;
						pos.z = z - half.z - E;
						break;
					}
				}
			}
		}

		if (vel.y < 0.0) {
			for (int x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
				bool broken = false;
				for (int z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
					int y = floor(pos.y - half.y - E);
					if (isBlockObstacle(blockMap, x, y, z)) {
						vel.y *= 0.0;
						pos.y = y + 1 + half.y;
						int f = DEFAULT_FRICTION;
						vel.x *= std::max(0.0, 1.0 - dt * f);
						vel.z *= std::max(0.0, 1.0 - dt * f);
						hitbox->isGrounded = true;
						broken = true;
						break;
					}
				}
				if (broken)
					break;
			}
		}
		if (vel.y > 0.0) {
			for (int x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
				for (int z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
					int y = floor(pos.y + half.y + E);
					if (isBlockObstacle(blockMap, x, y, z)) {
						vel.y *= 0.0;
						pos.y = y - half.y - E;
						break;
					}
				}
			}
		}

		vel.x *= std::max(0.0, 1.0 - dt * linearDump);
		vel.z *= std::max(0.0, 1.0 - dt * linearDump);

		pos.x += vel.x * dt;
		pos.y += vel.y * dt;
		pos.z += vel.z * dt;
	}
}

bool MKGame::isBlockInside(int x, int y, int z, MKGame::Hitbox* hitbox) {
	glm::vec3& pos = hitbox->pos;
	glm::vec3& half = hitbox->halfSize;
	return x >= floor(pos.x - half.x) && x <= floor(pos.x + half.x) &&
		z >= floor(pos.z - half.z) && z <= floor(pos.z + half.z) &&
		y >= floor(pos.y - half.y) && y <= floor(pos.y + half.y);
}
