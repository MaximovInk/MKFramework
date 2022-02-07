#pragma once
#include<glad/gl.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "API.H"

class MK_ENGINE_API camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 Matrix = glm::mat4(1.0f);
	float sensitivity = 100.0f;
	float aspectRation = 1.0f;

	camera(glm::vec3 position);

	void updateMatrix(float FOVdeg, float nearPlane, float farPlane, float aspectRation);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjMatrix();

	void moveRight(float force);
	void moveForward(float force);
	void moveUp(float force);

	void mouseMove(float mx,float my);
	//void handleInput(mkWindow* wnd, float deltaTime);

private:
	glm::mat4 view;
	glm::mat4 projection;
};
