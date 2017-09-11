#include "stdafx.h"

#include "Image.h"
#include "Graphics.h"
#include "Input.h"

#include "Application.h"

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
	
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
		if (renderer == nullptr)
		{
			cout << "Failed to create renderer : " << SDL_GetError() << endl;
			is_failed = true;
		}
	
		graphics = new Graphics(renderer);
		input = new Input();
	}
	
	Application::~Application()
	{
		delete input;
		delete graphics;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	int Application::Run()
	{
		if (is_failed)
		{
			return 1;
		}

		SDL_RenderSetLogicalSize(renderer, size.w, size.h);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		Image *hero = graphics->NewImage("hero.bmp");
		Image *block = new Image(Size(128), Color(0, 255));
		block->FillRect(Point(4), Size(120), Color(255, 0));

		float angle = 0.f;
		Point pos(100, 300);

		while (!input->IsWillQuit())
		{
			input->Update();

			angle += 0.1f;

			if (input->IsKeyDown(SDL_SCANCODE_W))
			{
				pos.y -= 10;
			}
			if (input->IsKeyDown(SDL_SCANCODE_S))
			{
				pos.y += 10;
			}
			if (input->IsKeyPressed(SDL_SCANCODE_A))
			{
				pos.x -= 10;
			}
			if (input->IsKeyPressed(SDL_SCANCODE_D))
			{
				pos.x += 10;
			}

			graphics->Begin();
			graphics->DrawImage(hero, pos, angle);
			graphics->DrawImage(block, Point(300, 100 + static_cast<uint32_t>(angle)));
			graphics->End();
		}

		delete block;
		delete hero;
	
		return 0;
	}
}
