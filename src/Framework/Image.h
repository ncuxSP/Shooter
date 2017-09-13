#pragma once

namespace Engine
{
	class Image
	{
	public:
		Image(const string &_filename);
		Image(const Size &_size, const Color &_color);
		~Image();

		void FillRect(const Point &_position, const Size &_size, const Color &_color);
		const Size &GetSize() const;

	private:
		friend class Graphics;

		void Init(SDL_Renderer *_renderer);

	private:
		SDL_Surface *surface;
		SDL_Texture *texture;

		Size size;

		bool is_inited;
	};

	inline const Size &Image::GetSize() const
	{
		return size;
	}
}

