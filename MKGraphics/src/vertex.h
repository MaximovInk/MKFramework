#pragma once

#include<glm/glm.hpp>

struct vertex
{
	glm::vec3 position;		//0
	glm::vec3 normal;		//1
	glm::vec3 color;		//2
	glm::vec2 texUV;		//3
	glm::vec3 aTangent;		//4
	glm::vec3 aBitangent;	//5
};
