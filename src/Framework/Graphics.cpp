#include "stdafx.h"

#include "SDL.h"
#include "Image.h"

#include "Graphics.h"

namespace Engine
{
	Graphics::Graphics(SDL_Renderer *_renderer)
		:	renderer(_renderer)
	{
	}
	
	Image * Graphics::NewImage(const string &_file_name)
	{
		return new Image(_file_name);
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
