#pragma once

#include "SDL_image.h"
#include "SDL_ttf.h"

namespace Engine
{
	class Image
	{
	public:
		Image(const string &_filename);
		Image(const Size &_size, const Color &_color);
		virtual ~Image();

		void FillRect(const Point &_position, const Size &_size, const Color &_color);
		const Size &GetSize() const;

	private:
		friend class Graphics;
		void RenderTexture(SDL_Renderer *_renderer);

	protected:
		Image();
		SDL_Surface *surface;
		SDL_Texture *texture;

		Size size;
		bool is_inited;
		bool is_cached;
	};

	inline const Size &Image::GetSize() const
	{
		return size;
	}

	class ImageCache
	{
	public:
		static ImageCache &GetInstance();

		SDL_Surface *Get(const string &_filename);

		void Flush();

		~ImageCache();

	private:
		ImageCache() = default;
		ImageCache(const ImageCache &) = default;
		ImageCache& operator=(ImageCache &) = default;

	private:
		unordered_map<string, SDL_Surface *> cache;
	};

	class ImageText : public Image
	{
	public:
		ImageText(const string &_text, const Size &_size, const Color &_color);
		virtual ~ImageText() override;
		
		void SetText(const string &_text);
		
		void SetSize(const Size &_size);

	private:
		void RenderTextSurface();

	private:
		TTF_Font *font;
		string text;
		Color color;
	};
}

