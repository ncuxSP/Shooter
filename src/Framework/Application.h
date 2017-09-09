#pragma once

class Application
{
public:
	Application(const string &_name, uint32_t _size_x, uint32_t _size_y);
	~Application();

	int Run();

private:
	class Graphics *graphics;
	class SDL_Window *window;
	class SDL_Renderer *renderer;
	bool is_failed;
};

