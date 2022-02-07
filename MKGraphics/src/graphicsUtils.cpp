#include "graphicsUtils.h"
#include "MKGRAPHICS.H"

MKEngine::mesh* MKEngine::Utils::makeQuad(glm::vec2 min,glm::vec2 max, MKGraphics::Axis axis)
{
	glm::vec3 pos0;
	glm::vec3 pos1;
	glm::vec3 pos2;
	glm::vec3 pos3;

	if (axis == MKGraphics::AXIS_X) {
		pos0 = glm::vec3(0, min.x, min.y);
		pos1 = glm::vec3(0, min.x, max.y);
		pos2 = glm::vec3(0, max.x, max.y);
		pos3 = glm::vec3(0, max.x, min.y);
	}
	if (axis == MKGraphics::AXIS_Y) {
		pos0 = glm::vec3(min.x, 0, min.y);
		pos1 = glm::vec3(min.x, 0, max.y);
		pos2 = glm::vec3(max.x, 0, max.y);
		pos3 = glm::vec3(max.x, 0, min.y);
	}
	if (axis == MKGraphics::AXIS_Z) {
		pos0 = glm::vec3(min.x, min.y, 0);
		pos1 = glm::vec3(min.x, max.y, 0);
		pos2 = glm::vec3(max.x, max.y, 0);
		pos3 = glm::vec3(max.x, min.y, 0);
	}

	std::vector<vertex> verts{
		vertex{pos0, glm::vec3(0),glm::vec3(1), glm::vec2(0,0)},
		vertex{pos1, glm::vec3(0),glm::vec3(1), glm::vec2(0,1)},
		vertex{pos2, glm::vec3(0),glm::vec3(1), glm::vec2(1,1)},
		vertex{pos3, glm::vec3(0),glm::vec3(1), glm::vec2(1,0)}
	};
	std::vector<GLuint> ind{ 0,1,2,0,2,3 };

	return new MKEngine::mesh(verts, ind);
}

MKEngine::mesh* MKEngine::Utils::makeCube()
{

	std::vector<vertex> verts{
		vertex{glm::vec3(0,0,0), glm::vec3(0),glm::vec3(1), glm::vec2(0,0)},
		vertex{glm::vec3(0,1,0), glm::vec3(0),glm::vec3(1), glm::vec2(0,1)},
		vertex{glm::vec3(1,1,0), glm::vec3(0),glm::vec3(1), glm::vec2(1,1)},
		vertex{glm::vec3(1,0,0), glm::vec3(0),glm::vec3(1), glm::vec2(1,0)},
		
		vertex{glm::vec3(0,0,0), glm::vec3(0),glm::vec3(1), glm::vec2(0,0)},
		vertex{glm::vec3(0,0,1), glm::vec3(0),glm::vec3(1), glm::vec2(0,1)},
		vertex{glm::vec3(0,1,1), glm::vec3(0),glm::vec3(1), glm::vec2(1,1)},
		vertex{glm::vec3(0,1,0), glm::vec3(0),glm::vec3(1), glm::vec2(1,0)},
		
		vertex{glm::vec3(0,0,0), glm::vec3(0),glm::vec3(1), glm::vec2(0,0)},
		vertex{glm::vec3(1,0,0), glm::vec3(0),glm::vec3(1), glm::vec2(0,1)},
		vertex{glm::vec3(1,0,1), glm::vec3(0),glm::vec3(1), glm::vec2(1,1)},
		vertex{glm::vec3(0,0,1), glm::vec3(0),glm::vec3(1), glm::vec2(1,0)},

		vertex{glm::vec3(1,0,1), glm::vec3(0),glm::vec3(1), glm::vec2(0,0)},
		vertex{glm::vec3(1,1,1), glm::vec3(0),glm::vec3(1), glm::vec2(0,1)},
		vertex{glm::vec3(0,1,1), glm::vec3(0),glm::vec3(1), glm::vec2(1,1)},
		vertex{glm::vec3(0,0,1), glm::vec3(0),glm::vec3(1), glm::vec2(1,0)},

		vertex{glm::vec3(1,1,0), glm::vec3(0),glm::vec3(1), glm::vec2(0,0)},
		vertex{glm::vec3(1,1,1), glm::vec3(0),glm::vec3(1), glm::vec2(0,1)},
		vertex{glm::vec3(1,0,1), glm::vec3(0),glm::vec3(1), glm::vec2(1,1)},
		vertex{glm::vec3(1,0,0), glm::vec3(0),glm::vec3(1), glm::vec2(1,0)},

		vertex{glm::vec3(0,1,1), glm::vec3(0),glm::vec3(1), glm::vec2(0,0)},
		vertex{glm::vec3(1,1,1), glm::vec3(0),glm::vec3(1), glm::vec2(0,1)},
		vertex{glm::vec3(1,1,0), glm::vec3(0),glm::vec3(1), glm::vec2(1,1)},
		vertex{glm::vec3(0,1,0), glm::vec3(0),glm::vec3(1), glm::vec2(1,0)}
	};

	std::vector<GLuint> ind;

	for (size_t i = 0; i < 6; i++)
	{
		int index = i * 4;
		ind.push_back(index);
		ind.push_back(index+1);
		ind.push_back(index+2);
		ind.push_back(index);
		ind.push_back(index+2);
		ind.push_back(index+3);
	}

	return new MKEngine::mesh(verts,ind);
}