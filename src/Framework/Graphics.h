#pragma once

namespace Engine
{
	class Graphics
	{
	public:
		Graphics(SDL_Renderer *_renderer);
	
		Image *NewImage(const string &_file_name);
		void Begin();
		void DrawImage(Image *_img, const Point &_position);
		void End();
	
	private:
		SDL_Renderer *renderer;
	};
}

