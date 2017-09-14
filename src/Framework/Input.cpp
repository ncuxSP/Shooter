#include "stdafx.h"

namespace Engine
{

	Input::Input()
		:	is_will_quit(false)
		,	keyboard(nullptr)
	{

	}

	void Input::Update()
	{
		for (uint32_t i = 0; i < SDL_NUM_SCANCODES; ++i)
		{
			key_down[i] = false;
			key_up[i] = false;
		}

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				is_will_quit = true;
				break;
			case SDL_KEYDOWN:
			{
				keyboard = SDL_GetKeyboardState(nullptr);
				auto index = event.key.keysym.scancode;
				key_down[index] = true;
			}
				break;
			case SDL_KEYUP:
			{
				keyboard = SDL_GetKeyboardState(nullptr);
				auto index = event.key.keysym.scancode;
				key_up[index] = true;
			}
				break;
			case SDL_TEXTINPUT:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
			default:
				break;
			}
		}
	}

	bool Input::IsKeyPressed(uint32_t _key) const
	{
		if (keyboard == nullptr)
		{
			return false;
		}
		return keyboard[_key] > 0;
	}

	bool Input::IsKeyDown(uint32_t _key) const
	{
		return key_down[_key];
	}

	bool Input::IsKeyUp(uint32_t _key) const
	{
		return key_up[_key];
	}

}