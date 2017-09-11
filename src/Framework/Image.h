#pragma once

namespace Engine
{
	class Image
	{
	public:
		Image(const string &_filename);
		Image(const Size &_size);
		~Image();

	private:
		friend class Graphics;

		void Init(SDL_Renderer *_renderer);

	private:
		SDL_Surface *surface;
		SDL_Texture *texture;

		Size size;

		bool is_inited;
	};
}

