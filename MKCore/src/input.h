#pragma once
#include "sdl/SDL.h"
#include "API.h"

namespace MKEngine
{
	namespace Input{

		MK_ENGINE_API int getMouseX();
		MK_ENGINE_API int getMouseY();
		MK_ENGINE_API int getMouseDeltaX();
		MK_ENGINE_API int getMouseDeltaY();
		MK_ENGINE_API int getMouseScrollDelta();
		MK_ENGINE_API void getMousePos(int* x, int* y);

		MK_ENGINE_API bool getMouseButton(int button);
		MK_ENGINE_API bool getMouseButtonDown(int button);
		MK_ENGINE_API bool getMouseButtonUp(int button);

		MK_ENGINE_API bool getKey(SDL_Keycode key);
		MK_ENGINE_API bool getKeyDown(SDL_Keycode key);
		MK_ENGINE_API bool getKeyUp(SDL_Keycode key);

		MK_ENGINE_API void eventUpdate(SDL_Event event);
		MK_ENGINE_API void update();
	}
}