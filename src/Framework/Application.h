#pragma once

namespace Engine
{
	class Application
	{
	public:
		Application(const string &_name, const Size &_size);
		virtual ~Application();

		int Run();

		virtual void OnBegin() = 0;
		virtual void OnUpdate(float _dt) = 0;
		virtual void OnRender() = 0;
		virtual void OnEnd() = 0;

	protected:
		Graphics *graphics;
		Input *input;
		Size size;

	private:
		SDL_Window *window;
		bool is_failed;
	};
}

