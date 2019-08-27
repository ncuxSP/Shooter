#include "stdafx.h"

namespace Engine
{
	Application::Application(const string &_name, const Size &_size)
		: is_failed(false)
		, size(_size)
	{
		const Point pos(100);

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			cout << " Failed to initialize SDL : " << SDL_GetError() << endl;
			is_failed = true;
		}

		window = SDL_CreateWindow(_name.c_str(), pos.x, pos.y, _size.x, _size.y, SDL_WINDOW_SHOWN);

		if (window == nullptr)
		{
			cout << "Failed to create window : " << SDL_GetError() << endl;
			is_failed = true;
		}

		graphics = new Graphics(window, size);
		input = new Input();

		int img_flags = IMG_INIT_PNG;
		if (!(IMG_Init(img_flags) & img_flags))
		{
			cout << " Failed to initialize SDL_image : " << IMG_GetError() << endl;
			is_failed = true;
		}

		if (TTF_Init() == -1)
		{
			cout << " Failed to initialize SDL_image : " << IMG_GetError() << endl;
			is_failed = true;
		}
	}

	Application::~Application()
	{
		delete input;
		delete graphics;

		SDL_DestroyWindow(window);

		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
	}

	int Application::Run()
	{
		if (is_failed)
		{
			system("Pause");
			return 1;
		}

		using Time = chrono::high_resolution_clock;
		using Seconds = chrono::duration<float>;

		OnBegin();

		auto last_tick = Time::now();

		while (!input->IsWillQuit())
		{
			input->Update();

			auto current_tick = Time::now();
			Seconds dt = current_tick - last_tick;
			last_tick = current_tick;

			OnUpdate(dt.count());

			graphics->Begin();
			OnRender();
			graphics->End();
		}

		OnEnd();

		return 0;
	}

}
