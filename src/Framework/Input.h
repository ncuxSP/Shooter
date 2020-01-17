#pragma once

namespace Engine
{
	const uint32_t KEYS_COUNT = 282;

	class Input
	{
	public:
		Input();

		void Update();

		bool IsKeyPressed(uint32_t _key) const;
		bool IsKeyDown(uint32_t _key) const;
		bool IsKeyUp(uint32_t _key) const;

		bool IsWillQuit() const
		{
			return is_will_quit;
		}

	private:
		bool is_will_quit;
		bool key_down[SDL_NUM_SCANCODES];
		bool key_up[SDL_NUM_SCANCODES];
		const uint8_t *keyboard;
	};
}
