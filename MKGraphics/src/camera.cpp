#include "camera.h"

camera::camera(glm::vec3 position)
{
	Position = position;
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
}

void camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane,float aspectRation)
{
	this->aspectRation = aspectRation;
	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), aspectRation, nearPlane, farPlane);

	Matrix = projection * view;
}

glm::mat4 camera::getViewMatrix()
{
	return view;
}

glm::mat4 camera::getProjMatrix()
{
	return projection;
}


void camera::mouseMove(float rotX, float rotY)
{
	rotX *= sensitivity;
	rotY *= sensitivity;

	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		Orientation = newOrientation;
	}

	Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
}