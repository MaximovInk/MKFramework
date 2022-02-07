#include "window.h"
#include "application.h"
#include <glad/gl.h>

namespace MKEngine {
	window::window(const char* title, int x, int y, int width, int height, int sdlFlags)
	{
		windowPtr_ = SDL_CreateWindow(title, x, y, width, height, sdlFlags);
		sdlFlags_ = sdlFlags;

		if (windowPtr_ == nullptr) {
			//spdlog::error("Could not create window: {}", SDL_GetError());
			return;
		}

		windowId_ = SDL_GetWindowID(windowPtr_);
		
		//application::checkGL(this);

		//spdlog::info("Window ({}) created successfully", title);

		application::addWindow(this);
	}

	void window::destroy()
	{
		if (destroyCallback_)
			destroyCallback_(this);

		if (windowPtr_ != nullptr)
			SDL_DestroyWindow(windowPtr_);

		renderCallback_ = nullptr;
		updateCallback_ = nullptr;
		sdlEventCallback_ = nullptr;
		destroyCallback_ = nullptr;

		windowPtr_ = nullptr;

	}

	unsigned int window::getWindowAppID()
	{
		return windowId_;
	}

	int window::getWidth()
	{
		int w;
		int h;
		SDL_GetWindowSize(windowPtr_, &w, &h);
		return w;
	}

	int window::getHeight()
	{
		int w;
		int h;
		SDL_GetWindowSize(windowPtr_, &w, &h);
		return h;
	}

	void window::getSize(int* width, int* height)
	{
		SDL_GetWindowSize(windowPtr_, width, height);
	}

	const char* window::getTitle()
	{
		return nullptr;
	}

	SDL_Window* window::getNativeWindow()
	{
		return windowPtr_;
	}

	void window::setTitle(const char* newTitle)
	{
		SDL_SetWindowTitle(windowPtr_, newTitle);
	}

	void window::show()
	{
		SDL_ShowWindow(windowPtr_);
	}

	void window::hide()
	{
		SDL_HideWindow(windowPtr_);
	}

	void window::setBordered(bool value)
	{
		SDL_SetWindowBordered(windowPtr_, value ? SDL_TRUE : SDL_FALSE);
	}

	void window::setFullscreen(bool value)
	{
		SDL_SetWindowFullscreen(windowPtr_, value ? SDL_WINDOW_FULLSCREEN : ~SDL_WINDOW_FULLSCREEN);
	}

	void window::makeGLCurrent()
	{
		SDL_GL_MakeCurrent(windowPtr_, application::getGLContext());
	}

	bool window::operator==(const window& other)
	{
		return windowId_ == other.windowId_;
	}

	void window::setUpdateCallback(std::function<void(window*, float deltaTime)>func)
	{
		updateCallback_ = func;
	}

	void window::setRenderCallback(std::function<void(window*, float deltaTime)> func)
	{
		renderCallback_ = func;
	}

	void window::setSDLEventCallback(std::function<void(window*, SDL_Event sdlEvent)>func)
	{
		sdlEventCallback_ = func;
	}

	void window::setDestroyCallback(std::function<void(window*)> func)
	{
		destroyCallback_ = func;
	}

	void window::processEvents(SDL_Event event)
	{
		if (sdlEventCallback_)
			sdlEventCallback_(this, event);
		switch (event.type)
		{
		case SDL_QUIT:
			destroy();
			break;

		default:
			break;
		}
	}

	void window::update(double deltaTime)
	{
		if (updateCallback_)
			updateCallback_(this, deltaTime);
	}

	void window::render(double deltaTime)
	{
		SDL_GL_MakeCurrent(windowPtr_, application::getGLContext());
		if(renderCallback_)
			renderCallback_(this, deltaTime);

		

		SDL_GL_SwapWindow(windowPtr_);
	}

}