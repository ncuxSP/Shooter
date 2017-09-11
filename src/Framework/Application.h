#pragma once

namespace Engine
{
	class Application
	{
	public:
		Application(const string &_name, const Size &_size);
		~Application();
	
		int Run();
	
	private:
		class Graphics *graphics;
		class SDL_Window *window;
		class SDL_Renderer *renderer;
		bool is_failed;
		Size size;
	};
}

