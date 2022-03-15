#include "graphicsUtils.h"
#include "MKGRAPHICS.H"
#include "string"
#include "MKUtils.h"

MKEngine::mesh* quad;
shader* colorShader;
shader* texShader;

std::string solutionDir = "C:\\Users\\Danila\\Documents\\Github\\MKFramework\\";

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

float lineWidth = 0.5f;

void MKEngine::Utils::setLineWidth(float width)
{
	lineWidth = width;
}

void checkInit() {
	if (quad == nullptr) {
		quad = MKEngine::Utils::makeQuad(glm::vec2(0,0), glm::vec2(1,1), MKGraphics::AXIS_Z);
	}
	std::string resDir = solutionDir + "resources\\shaders\\";

	if (colorShader == nullptr) {
		colorShader = new shader((resDir+"baseShader.vert").c_str(), (resDir + "baseShader.frag").c_str());
	}
	if (texShader == nullptr) {
		texShader = new shader((resDir + "baseShader.vert").c_str(), (resDir + "texShader.frag").c_str());
	}
}

void MKEngine::Utils::drawLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color,camera*cam)
{
	checkInit();

	auto length = glm::length(pos2 - pos1);
	LOG::info(length);
	
	auto dir = glm::normalize(pos2 - pos1);
	
	auto camMatrix = cam->Matrix;
	auto model = glm::mat4(1.0);
	/*const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
		dir.x,
		glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
		dir.y,
		glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
		dir.z,
		glm::vec3(0.0f, 0.0f, 1.0f));

	// Y * X * Z
	const glm::mat4 rotationMatrix = transformY * transformX * transformZ;
	*/

	glm::vec3 up(0, 1, 0);
	glm::vec3 z_axis = glm::normalize(dir);
	glm::vec3 x_axis = glm::normalize(glm::cross(up, z_axis));
	glm::vec3 y_axis = glm::normalize(glm::cross(z_axis, x_axis));

	glm::mat4 rotationMatrix(
		x_axis.x,x_axis.y,x_axis.z,0,
		y_axis.x, y_axis.y, y_axis.z, 0,
		z_axis.x, z_axis.y, z_axis.z, 0,
		0,0,0,1
	);


	model = glm::translate(glm::mat4(1.0f), pos1) * rotationMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(lineWidth, 1.0, length));

	colorShader->use();
	colorShader->setMat4("camMatrix", camMatrix);
	colorShader->setMat4("model", model);
	colorShader->setVec4("color", color);
	colorShader->setVec3("camPos", cam->Position);

	quad->draw();

}

void MKEngine::Utils::drawLine(glm::vec3 pos1, glm::vec3 pos2, texture texure)
{
	checkInit();


}

void MKEngine::Utils::drawQuad(glm::vec3 pos, glm::vec3 rot, glm::vec2 size, glm::vec4 color, camera* cam)
{
	checkInit();

	auto camMatrix = cam->Matrix;
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
	colorShader->setVec3("camPos", cam->Position);

	quad->draw();

}

void MKEngine::Utils::drawQuad(glm::vec3 pos, glm::vec4 size, texture texure)
{
	checkInit();
}


