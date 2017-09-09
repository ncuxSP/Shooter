#pragma once

class Graphics
{
public:
	Graphics(SDL_Renderer *_renderer);

	Image *NewImage(const string &_file_name);
	bool DrawImage(Image *_img, uint32_t _x, uint32_t _y);

private:
	SDL_Renderer *renderer;
};

