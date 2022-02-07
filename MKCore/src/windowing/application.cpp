#include "application.h"
//#include "glad/gl.h"
#include "MKUtils.h"
#include "../input.h"

namespace MKEngine {
	static Uint32 startTime_;
	static int eventUpdateType_;
	static SDL_GLContext glContext;
	static std::vector<window*> windows_;

	int application::init()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			//spdlog::critical("SDL INIT ERROR: {}", SDL_GetError());
			return 1;
		}

		//SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		//SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		//SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}

	void application::finalize()
	{
		for (auto& window : windows_)
		{
			window->destroy();
		}

		windows_.clear();

		SDL_GL_DeleteContext(glContext);
		SDL_Quit();
	}

	void application::startLoop()
	{
		Uint32 last = 0;
		double elapsed = 0;

		while (updateWindows(elapsed))
		{
			const auto current = SDL_GetTicks();
			elapsed = (current - last) / 1000.0;

			last = current;
			startTime_ = current;
		}
	}

	unsigned int application::getTicks()
	{
		return startTime_;
	}

	SDL_GLContext application::getGLContext()
	{
		return glContext;
	}

	void application::setGLContext(SDL_GLContext context)
	{
		glContext = context;
	}

	bool application::updateWindows(double deltaTime)
	{
		SDL_Event event;

		auto is_available = SDL_PollEvent(&event);

		Input::update();
		if (is_available)
			Input::eventUpdate(event);

		for (auto& window : windows_)
		{
			window->update(deltaTime);
		}
		//LOG::info(windows_.size());
		for (auto& window : windows_)
		{
			window->render(deltaTime);
		}

		if (is_available) {
			for (auto* element : windows_)
			{
				if ((element->windowId_ == event.window.windowID)) {
					element->processEvents(event);
				}
			}
		}

		return true;
	}

	void application::addWindow(MKEngine::window* window)
	{
		windows_.push_back(window);
	}

	void application::removeWindow(MKEngine::window* window)
	{
		if (windows_.size() < 2) {
			windows_.clear();
			return;
		}

		windows_.erase(remove(windows_.begin(), windows_.end(), window), windows_.end());
	}
	/*
	
	void application::checkGL(window* window)
	{
		if (glContext == nullptr)
		{
			//glContext = SDL_GL_CreateContext(window->getNativeWindow());
			//const auto version = gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
			//const auto version = gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
			//SDL_GL_LoadLibrary(nullptr);
			//spdlog::info("GL {0}.{1}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
		}
	}
	*/
}