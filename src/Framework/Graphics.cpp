#include "stdafx.h"

namespace Engine
{
	Graphics::Graphics(SDL_Window *_window, const Size &_size)
		:	renderer(nullptr)
	{
		renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (renderer == nullptr)
		{
			cout << "Failed to create renderer : " << SDL_GetError() << endl;
		}

		SDL_RenderSetLogicalSize(renderer, _size.w, _size.h);
	}
	
	Graphics::~Graphics()
	{
		SDL_DestroyRenderer(renderer);
	}

	Image * Graphics::NewImage(const string &_file_name)
	{
		return new Image(_file_name);
	}
	
	void Graphics::SetColor(const Color &_color)
	{
		SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
	}

	void Graphics::Begin()
	{
		SDL_RenderClear(renderer);
	}

	void Graphics::DrawImage(Image *_img, const Point &_position)
	{
		_img->Init(renderer);

		SDL_Rect src{ 0, 0, _img->size.w, _img->size.h };
		SDL_Rect dst{ _position.x, _position.y, _img->size.w, _img->size.h };

		SDL_RenderCopy(renderer, _img->texture, &src, &dst);
	}

	void Graphics::DrawImage(Image *_img, const Point &_position, float _angle)
	{
		_img->Init(renderer);

		SDL_Rect src{ 0, 0, _img->size.w, _img->size.h };
		SDL_Rect dst{ _position.x, _position.y, _img->size.w, _img->size.h };

		SDL_RenderCopyEx(renderer, _img->texture, &src, &dst, _angle, nullptr, SDL_FLIP_NONE);
	}

	void Graphics::End()
	{
		SDL_RenderPresent(renderer);
	}

}
