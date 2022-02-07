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

void camera::moveRight(float force)
{
	Position += force * glm::normalize(glm::cross(Orientation, Up));
}

void camera::moveForward(float force)
{
	Position += force * Orientation;
}

void camera::moveUp(float force)
{
	Position += force * Up;
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




/*

void camera::handleInput(mkWindow* wnd, float deltaTime)
{
	const Uint8* kb = SDL_GetKeyboardState(NULL);

	auto force = deltaTime;
	if (kb[SDL_SCANCODE_LSHIFT])
	{
		force *= speed * 2;
	}
	else {
		force *= speed;
	}

	if (kb[SDL_SCANCODE_W])
	{
		Position += force * Orientation;
	}

	if (kb[SDL_SCANCODE_A])
	{
		Position += force * -glm::normalize(glm::cross(Orientation, Up));
	}

	if (kb[SDL_SCANCODE_S])
	{
		Position += force * -Orientation;
	}

	if (kb[SDL_SCANCODE_D])
	{
		Position += force * glm::normalize(glm::cross(Orientation, Up));
	}

	if (kb[SDL_SCANCODE_SPACE])
	{
		Position += force * Up;
	}
	if (kb[SDL_SCANCODE_LCTRL])
	{
		Position += force * -Up;
	}


	int mx;
	int my;

	Uint32 buttons = SDL_GetMouseState(&mx, &my);


	if ((buttons & SDL_BUTTON_LMASK) != 0)
	{
		// Hides mouse cursor


		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			SDL_ShowCursor(false);
			SDL_WarpMouseInWindow(wnd->getNativeWindow(), (wnd->getWidth() / 2), (wnd->getHeight() / 2));
			firstClick = false;
		}
		else
		{
			SDL_GetMouseState(&mx, &my);

			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees
			float rotX = sensitivity * (float)(my - (wnd->getHeight() / 2)) / wnd->getHeight();
			float rotY = sensitivity * (float)(mx - (wnd->getWidth() / 2)) / wnd->getWidth();

			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

			// Decides whether or not the next vertical Orientation is legal or not
			if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				Orientation = newOrientation;
			}

			// Rotates the Orientation left and right
			Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);


		}

		SDL_WarpMouseInWindow(wnd->getNativeWindow(), (wnd->getWidth() / 2), (wnd->getHeight() / 2));
	}
	else if ((buttons & SDL_BUTTON_LMASK) == 0)
	{
		SDL_ShowCursor(true);
		firstClick = true;
	}

}

*/