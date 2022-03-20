#include "graphicsUtils.h"
#include "MKGRAPHICS.H"
#include "string"
#include "MKUtils.h"

MKEngine::mesh* quad;
MKEngine::mesh* lineQuad;
shader* colorShader;
shader* texShader;

std::string baseShaderFragCol = "#version 330 core\n\
out vec4 FragColor;\n\
uniform vec4 color;\n\
void main()\n\
{\n\
	FragColor = color;\n\
}; ";
std::string baseShaderFragTex = "#version 330 core\n\
out vec4 FragColor;\n\
uniform sampler2D diffuse0;\n\
in vec2 texCoords;\n\
void main()\n\
{\n\
	vec3 color = texture(diffuse0, texCoords).rgb;\n\
	FragColor = vec4(color, 1.0f);\n\
};";
std::string baseShaderVert = "#version 330 core\n\
layout(location = 0) in vec3 aPos;\n\
layout(location = 1) in vec3 aNormal;\n\
layout(location = 2) in vec3 aColor;\n\
layout(location = 3) in vec2 aTex;\n\
out vec2 texCoords;\n\
uniform mat4 camMatrix; \n\
uniform mat4 model;\n\
void main()\n\
{\n\
	texCoords = aTex; \n\
	gl_Position = camMatrix * model * vec4(aPos, 1.0); \n\
}";

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

void MKEngine::Utils::setLineWidth(float width)
{
	glLineWidth(width);
}
glm::mat4 camMatrix(1.0);
void MKEngine::Utils::setCamMatrix(glm::mat4 matrix)
{
	camMatrix = matrix;
}

void checkInit() {
	if (quad == nullptr) {
		quad = MKEngine::Utils::makeQuad(glm::vec2(0,0), glm::vec2(1,1), MKGraphics::AXIS_Y);
	}

	if (lineQuad == nullptr) {
		lineQuad = MKEngine::Utils::makeQuad(glm::vec2(0, 0), glm::vec2(1, 1), MKGraphics::AXIS_Y);
	}

	if (colorShader == nullptr) {
		colorShader = new shader(baseShaderVert.c_str(), baseShaderFragCol.c_str());
	}
	if (texShader == nullptr) {
		texShader = new shader(baseShaderVert.c_str(), baseShaderFragTex.c_str());
	}
}

void MKEngine::Utils::drawLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color)
{
	checkInit();

	auto length = glm::length(pos2 - pos1);
	
	auto dir = glm::normalize(pos1 - pos2);
	
	/*
	
	auto model = glm::mat4(1.0);
	float a_x = std::cos(dir.x);
	float a_y = std::sin(dir.y);
	float a_z = 0;

	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
		a_x,
		glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
		a_y,
		glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
		a_z,
		glm::vec3(0.0f, 0.0f, 1.0f));
	*/

	// Y * X * Z
	//const glm::mat4 rotationMatrix = transformY * transformX * transformZ;
	/*
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0,1,0), dir));
	glm::vec3 up = glm::cross(dir, right);
	//glm::vec3 up = glm::cross(pos1, pos2);

	const glm::mat4 rotationMatrix = glm::lookAt(glm::vec3(0,0,0), dir, up);
	*/

	//model = glm::translate(glm::mat4(1.0f), pos1) * rotationMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(lineWidth, 1.0, length));

	//quad->vertices = 

	/*
	float dx = pos1.x - pos2.x;
	float dy = pos1.x - pos2.x;
	float n = 2;
	float dst = std::sqrt(dx * dx + dy * dy);
	dx /= dst;
	dy /= dst;
	float x1 = pos1.x + (n / 2) * dy;
	float y1 = pos1.y - (n / 2) * dx;
	float x2 = pos1.x - (n / 2) * dy;
	float y2 = pos1.y + (n / 2) * dx;

	float x3 = pos2.x + (n / 2) * dy;
	float y3 = pos2.y - (n / 2) * dx;
	float x4 = pos2.x - (n / 2) * dy;
	float y4 = pos2.y + (n / 2) * dx;
	 

	*/

	lineQuad->vertices = std::vector<vertex>{
	vertex{glm::vec3(pos1.x,pos1.y,pos1.z), glm::vec3(0),glm::vec3(1), glm::vec2(0,0) },
	vertex{glm::vec3(pos2.x,pos2.y,pos2.z), glm::vec3(0),glm::vec3(1), glm::vec2(0,0) },
	};
	lineQuad->indices = std::vector<GLuint>{ 0,1 };

	lineQuad->applyData();

	

	colorShader->use();
	colorShader->setMat4("camMatrix", camMatrix);
	colorShader->setMat4("model", glm::mat4(1.0f));
	colorShader->setVec4("color", color);

	lineQuad->VAO.Bind();
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINES, lineQuad->indices.size(), GL_UNSIGNED_INT, 0);

	//lineQuad->draw();

}

void MKEngine::Utils::drawLine(glm::vec3 pos1, glm::vec3 pos2, texture texure)
{
	checkInit();


}

void MKEngine::Utils::drawQuad(glm::vec3 pos, glm::vec3 rot, glm::vec2 size, glm::vec4 color)
{
	checkInit();

	auto model = glm::mat4(1.0);
	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
		glm::radians(rot.x),
		glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
		glm::radians(rot.y),
		glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
		glm::radians(rot.z),
		glm::vec3(0.0f, 0.0f, 1.0f));

	// Y * X * Z
	const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

	model = glm::translate(glm::mat4(1.0f), pos) * rotationMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(size.x,1.0,size.y));

	colorShader->use();
	colorShader->setMat4("camMatrix", camMatrix);
	colorShader->setMat4("model", model);
	colorShader->setVec4("color", color);

	quad->draw();

}

void MKEngine::Utils::drawQuad(glm::vec3 pos, glm::vec4 size, texture texure)
{
	checkInit();
}

void MKEngine::Utils::beginRender()
{
}

void MKEngine::Utils::endRender()
{
}


