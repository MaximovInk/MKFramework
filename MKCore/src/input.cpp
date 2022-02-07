#include "input.h"

//#include "spdlog/spdlog.h"

int x = 0, y = 0;
int lastX, lastY;
int mouseScroll;

Uint8* keyboardState = 0;
int keyCount = 0;
Uint8* lastKeyboardState;

Uint32 mouseState = 0;
Uint32 lastMouseState;

int MKEngine::Input::getMouseX()
{
	return x;
}

int MKEngine::Input::getMouseY()
{
	return y;
}

int MKEngine::Input::getMouseDeltaX()
{
	return x - lastX;
}

int MKEngine::Input::getMouseDeltaY()
{
	return y - lastY;
}

int MKEngine::Input::getMouseScrollDelta()
{
	return mouseScroll;
}

void MKEngine::Input::getMousePos(int* _x, int* _y)
{
	*_x = x;
	*_y = y;
}

bool MKEngine::Input::getMouseButton(int button)
{
	return (mouseState & button) != 0;
}

bool MKEngine::Input::getMouseButtonDown(int button)
{
	return ((mouseState & button) != 0) && ((lastMouseState & button) == 0);
}

bool MKEngine::Input::getMouseButtonUp(int button)
{
	return ((mouseState & button) == 0) && ((lastMouseState & button) != 0);
}

bool MKEngine::Input::getKey(SDL_Keycode key)
{
	return keyboardState[key];
}

bool MKEngine::Input::getKeyDown(SDL_Keycode key)
{
	return keyboardState[key] && !lastKeyboardState[key];
}

bool MKEngine::Input::getKeyUp(SDL_Keycode key)
{
	return !keyboardState[key] && lastKeyboardState[key];
}

void MKEngine::Input::eventUpdate(SDL_Event event) {


	if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
	{
		mouseScroll = event.wheel.y;
	}

}

void MKEngine::Input::update()
{
	mouseScroll = 0;

	for (int i = 0; i < keyCount; i++) {
		lastKeyboardState[i] = keyboardState[i];
	}

	lastMouseState = mouseState;
	lastX = x;
	lastY = y;


	int kCount = 0;
	auto k = SDL_GetKeyboardState(&kCount);
	if (keyCount != kCount) {
		delete[] keyboardState;
		delete[] lastKeyboardState;
		keyCount = kCount;
		keyboardState = new Uint8[kCount];
		lastKeyboardState = new Uint8[kCount];
	}

	for (int i = 0; i < kCount; i++) {
		keyboardState[i] = k[i];
	}

	mouseState = SDL_GetMouseState(&x, &y);
}
