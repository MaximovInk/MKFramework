#pragma once

#include "sdl/SDL.h"
#include "window.h"
#include "../API.h"

namespace MKEngine {

	class window;

	class application {
	public:
		MK_ENGINE_API static int init();
		MK_ENGINE_API static void finalize();
		MK_ENGINE_API static void startLoop();

		MK_ENGINE_API static unsigned int getTicks();

		MK_ENGINE_API static SDL_GLContext getGLContext();

		MK_ENGINE_API static void setGLContext(SDL_GLContext context);
	private:
		

		static bool updateWindows(double deltaTime);
		static void addWindow(window* window);
		static void removeWindow(window* window);
		//static void checkGL(window* window);

		friend class window;
	};
}