#include "stdafx.h"

#include "SDL.h"

#include "Image.h"


namespace Engine
{	
	Image::Image(const string &_filename)
		:	texture(nullptr)
		,	is_inited(false)
	{
		surface = SDL_LoadBMP(_filename.c_str());
		if (surface == nullptr)
		{
			cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
		}
		size.w = surface->clip_rect.w;
		size.h = surface->clip_rect.h;
	}

	Image::Image(const Size &_size)
		:	texture(nullptr)
		,	is_inited(false)
		,	size(_size)
	{
		surface = SDL_CreateRGBSurface(0, size.w, size.h, 0, 0, 0, 0, 0);
		if (surface == nullptr)
		{
			cout << "SDL_CreateRGBSurface Error: " << SDL_GetError() << endl;
		}
	}

	Image::~Image()
	{
		SDL_FreeSurface(surface);
	}

	void Image::Init(SDL_Renderer *_renderer)
	{
		if (!is_inited)
		{
			texture = SDL_CreateTextureFromSurface(_renderer, surface);
			if (texture == nullptr)
			{
				cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
			}
			else
			{
				is_inited = true;
			}
		}
	}

}

