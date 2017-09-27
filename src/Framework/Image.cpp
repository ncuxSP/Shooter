#include "stdafx.h"

namespace Engine
{	
	Image::Image(const string &_filename)
		:	Image()
	{
		surface = ImageCache::GetInstance().Get(_filename);
		is_cached = true;
		if (surface == nullptr)
		{
			cout << "IMG_Load Error: " << IMG_GetError() << endl;
		}
		size.w = surface->clip_rect.w;
		size.h = surface->clip_rect.h;
	}

	Image::Image(const Size &_size, const Color &_color)
		:	Image()
	{
		size = _size;
		surface = SDL_CreateRGBSurface(0, size.w, size.h, 32, _color.r, _color.g, _color.b, _color.a);
		if (surface == nullptr)
		{
			cout << "SDL_CreateRGBSurface Error: " << SDL_GetError() << endl;
		}
	}

	Image::Image()
		:	texture(nullptr)
		,	surface(nullptr)
		,	is_inited(false)
		,	is_cached(false)
	{

	}

	Image::~Image()
	{
		if (!is_cached)
		{
			SDL_FreeSurface(surface);
		}
		if (texture != nullptr)
		{
			SDL_DestroyTexture(texture);
		}
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

	void Image::RenderTexture(SDL_Renderer *_renderer)
	{
		if (!is_inited)
		{
			if (texture != nullptr)
			{
				SDL_DestroyTexture(texture);
			}
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
	
	ImageCache &ImageCache::GetInstance()
	{
		static ImageCache instance;
		return instance;
	}

	SDL_Surface *ImageCache::Get(const string &_filename)
	{
		if (cache.count(_filename) == 0)
		{
			cache.insert({ _filename, IMG_Load(_filename.c_str()) });
		}
		return cache.at(_filename);
	}

	void ImageCache::Flush()
	{
		for (auto &p : cache)
		{
			SDL_FreeSurface(p.second);
		}
		cache.clear();
	}

	ImageCache::~ImageCache()
	{
		Flush();
	}

	ImageText::ImageText(const string &_text, const Size &_size, const Color &_color)
		:	Image()
		,	text(_text)
		,	color(_color)
	{
		font = TTF_OpenFont("data/arial.ttf", 48);
		size = _size;
		RenderTextSurface();
	}

	ImageText::~ImageText()
	{
		TTF_CloseFont(font);
	}

	void ImageText::SetText(const string &_text)
	{
		text = _text;
		RenderTextSurface();
	}

	void ImageText::SetSize(const Size &_size)
	{
	}

	void ImageText::RenderTextSurface()
	{
		is_inited = false;
		surface = TTF_RenderText_Blended(font, text.c_str(), { color.r, color.g, color.b, color.a });
	}

}

