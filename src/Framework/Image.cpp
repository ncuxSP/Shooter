#include "stdafx.h"

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

	Image::Image(const Size &_size, const Color &_color)
		:	texture(nullptr)
		,	is_inited(false)
		,	size(_size)
	{
		surface = SDL_CreateRGBSurface(0, size.w, size.h, 32, _color.r, _color.g, _color.b, _color.a);
		if (surface == nullptr)
		{
			cout << "SDL_CreateRGBSurface Error: " << SDL_GetError() << endl;
		}
	}

	Image::~Image()
	{
		SDL_FreeSurface(surface);
	}

	void Image::FillRect(const Point &_position, const Size &_size, const Color &_color)
	{
		is_inited = false;

		SDL_Rect dst 
		{
			_position.x, _position.y,
			_size.w, _size.h
		};
		SDL_FillRect(surface, &dst, SDL_MapRGBA(surface->format, _color.r, _color.g, _color.b, _color.a));
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

