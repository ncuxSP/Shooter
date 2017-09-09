#pragma once

class Image
{
public:
	Image(SDL_Texture *_texture);
	~Image();

	inline SDL_Texture *GetTexture() const
	{
		return texture;
	}

private:
	SDL_Texture *texture;
};

