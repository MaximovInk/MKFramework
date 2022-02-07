#pragma once
#include "sdl/SDL.h"
#include <functional>
#include "../API.h"

namespace MKEngine {

	class MK_ENGINE_API window {
	public:
		window(const char* title, int x, int y, int width, int height, int sdlFlags = SDL_WINDOW_OPENGL);
		void destroy();

		unsigned int getWindowAppID();
		int getWidth();
		int getHeight();
		void getSize(int* width, int* height);
		const char* getTitle();
		SDL_Window* getNativeWindow();
		void setTitle(const char* newTitle);
		void show();
		void hide();
		void setBordered(bool value);
		void setFullscreen(bool value);
		void makeGLCurrent();

		bool operator ==(const window& other);

		void setUpdateCallback(std::function<void(window*, float deltaTime) > );
		void setRenderCallback(std::function<void(window*, float deltaTime)>);
		void setSDLEventCallback(std::function<void(window*, SDL_Event sdlEvent)>);
		void setDestroyCallback(std::function<void(window*)>);
	private:
		SDL_Window* windowPtr_;

		Uint32 windowId_;
		Uint32 sdlFlags_;

		void processEvents(SDL_Event event);
		void update(double deltaTime);
		void render(double deltaTime);

		std::function<void(window*, float deltaTime)> updateCallback_ = nullptr;
		std::function<void(window*, SDL_Event sdlEvent)> sdlEventCallback_ = nullptr;
		std::function<void(window*)> destroyCallback_ = nullptr;
		std::function<void(window*, float deltaTime)> renderCallback_ = nullptr;

	friend class application;
	};
}