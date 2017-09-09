#include "stdafx.h"

#include "SDL.h"
#include "Image.h"
#include "Graphics.h"

#include "Application.h"

Application::Application(const string &_name, uint32_t _size_x, uint32_t _size_y)
	:	is_failed(false)
{
	const uint32_t pos_x = 100;
	const uint32_t pos_y = 200;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << " Failed to initialize SDL : " << SDL_GetError() << endl;
		is_failed = true;
	}

	window = SDL_CreateWindow(_name.c_str(), pos_x, pos_y, _size_x, _size_y, SDL_WINDOW_SHOWN);

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

	int sizeX = 300;
	int sizeY = 400;

	SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	Image *image = graphics->NewImage("hero.bmp");

	graphics->DrawImage(image, 100, 200);

	delete image;

	SDL_Delay(4000);

	return 0;
}
