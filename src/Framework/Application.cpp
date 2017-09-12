#include "stdafx.h"

namespace Engine
{
	Application::Application(const string &_name, const Size &_size)
		:	is_failed(false)
		,	size(_size)
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
	}
	
	Application::~Application()
	{
		delete input;
		delete graphics;
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	int Application::Run()
	{
		if (is_failed)
		{
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

	void Application::OnBegin()
	{

	}

	void Application::OnUpdate(float _dt)
	{

	}

	void Application::OnRender()
	{

	}

	void Application::OnEnd()
	{

	}

}
