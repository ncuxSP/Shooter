#include "stdafx.h"

#include "SDL.h"
#include "Image.h"
#include "Graphics.h"

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
	}
	
	Application::~Application()
	{
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
	
		Image *image = graphics->NewImage("hero.bmp");
	
		graphics->DrawImage(image, Point(100, 300));
	
		delete image;
	
		SDL_Delay(4000);
	
		return 0;
	}
}
