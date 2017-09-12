#pragma once

namespace Engine
{
	class Application
	{
	public:
		Application(const string &_name, const Size &_size);
		virtual ~Application();
	
		int Run();

		virtual void OnBegin();
		virtual void OnUpdate(float _dt);
		virtual void OnRender();
		virtual void OnEnd();

	protected:
		Graphics *graphics;
		Input *input;
		Size size;

	private:
		SDL_Window *window;
		bool is_failed;
	};
}

