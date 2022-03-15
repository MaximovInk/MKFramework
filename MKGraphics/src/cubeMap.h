#pragma once

#include "string"
#include "vector"
#include"camera.h"
#include "shader.h" 
#include "texture.h"
#include<glad/gl.h>

#include "API.H"

class MK_ENGINE_API cubeMap
{
public:
	cubeMap(std::vector<std::string> faces);

	void draw(shader& shader, camera& camera);
	void destroy();

private:
	GLuint ID;
	GLuint VAO;
	GLuint VBO;
};