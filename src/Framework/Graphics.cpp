#include "stdafx.h"

#include "SDL.h"
#include "Image.h"

#include "Graphics.h"

Graphics::Graphics(SDL_Renderer *_renderer)
	:	renderer(_renderer)
{
}

Image * Graphics::NewImage(const string &_file_name)
{
	SDL_Surface *surface = SDL_LoadBMP(_file_name.c_str());
	if (surface == nullptr)
	{
		cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
		return nullptr;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	if (texture == nullptr)
	{
		cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
		return nullptr;
	}

	Image *image = new Image(texture);
	if (image == nullptr)
	{
		cout << "Graphics::NewImage: Image could not be created" << endl;
		return nullptr;
	}
	
	return image;
}

bool Graphics::DrawImage(Image *_img, uint32_t _x, uint32_t _y)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, _img->GetTexture(), NULL, NULL);
	SDL_RenderPresent(renderer);

	return true;
}
