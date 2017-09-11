#pragma once

namespace Engine
{
	class Graphics
	{
	public:
		Graphics(SDL_Renderer *_renderer);
	
		Image *NewImage(const string &_file_name);
		bool DrawImage(Image *_img, const Point &_position);
	
	private:
		SDL_Renderer *renderer;
	};
}

