#include "stdafx.h"

#include "SDL.h"

#include "Image.h"


Image::Image(SDL_Texture *_texture)
	:	texture(_texture)
{

}

Image::~Image()
{

}

