#pragma once

namespace Engine
{
	class Graphics
	{
	public:
		Graphics(SDL_Window *_window, const Size &_size);
		~Graphics();
	
		Image *NewImage(const string &_file_name);
		void SetColor(const Color &_color);
		void Begin();
		void DrawImage(Image *_img, const Point &_position);
		void DrawImage(Image *_img, const Point &_position, float _angle);
		void End();
	
	private:
		SDL_Renderer *renderer;
	};
}

