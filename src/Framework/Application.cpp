#include "stdafx.h"

#include "Image.h"
#include "Graphics.h"
#include "Input.h"

#include "Framework/Entities.h"
#include "Framework/World.h"

#include "Application.h"

namespace Engine
{
	Application::Application(const string &_name, const Size &_size)
		:	is_failed(false)
		,	size(_size)
	{
		const Point pos(100);
	
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			cout << " Failed to initialize SDL : " << SDL_GetError() << endl;
			is_failed = true;
		}
	
		window = SDL_CreateWindow(_name.c_str(), pos.x, pos.y, _size.x, _size.y, SDL_WINDOW_SHOWN);
	
		if (window == nullptr)
		{
			cout << "Failed to create window : " << SDL_GetError() << endl;
			is_failed = true;
		}
	
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
		if (renderer == nullptr)
		{
			cout << "Failed to create renderer : " << SDL_GetError() << endl;
			is_failed = true;
		}
	
		graphics = new Graphics(renderer);
		input = new Input();
	}
	
	Application::~Application()
	{
		delete input;
		delete graphics;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	struct Rotation
	{
		Rotation(float _angle) : angle(_angle) {}

		float angle;
	};

	struct Position
	{
		Position(const Point &_pos) : pos(_pos) {}

		Point pos;
	};

	class RotationSystem : public EntitySystem, public EventSubscriber<Events::OnComponentAssigned<Rotation>>
	{
		virtual ~RotationSystem() override {}

		virtual void Configure(World* _world) override
		{
			_world->Subscribe<Events::OnComponentAssigned<Rotation>>(this);
		}

		virtual void UnConfigure(World* _world) override
		{
			_world->UnSubscribeAll(this);
		}

		virtual void Update(World* _world, float _dt) override
		{
			_world->Each<Rotation>([&](Entity *_entity, ComponentPtr<Rotation> _rot) -> void
			{
				_rot->angle += _dt;
			});
			_world->Each<Rotation, Position>([&](Entity *_entity, ComponentPtr<Rotation> _rot, ComponentPtr<Position> _pos) -> void
			{
				if (_rot->angle > 360.f)
				{
					_rot->angle = 180.f;
				}
				if (_pos->pos.x > 300.f)
				{
					_pos->pos.x = 100.f;
				}
			});
		}

		virtual void Receive(World *_world, const Events::OnComponentAssigned<Rotation> &_event) override
		{
			_event.component->angle = 30.f;
		}
	};
	
	int Application::Run()
	{
		if (is_failed)
		{
			return 1;
		}

		World world;
		world.RegisterSystem(new RotationSystem());

		auto e = world.Create();
		e->Assign<Rotation>(0.f);
		e->Assign<Position>(Point(100, 300));

		SDL_RenderSetLogicalSize(renderer, size.w, size.h);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		Image *hero = graphics->NewImage("hero.bmp");
		Image *block = new Image(Size(128), Color(0, 255));
		block->FillRect(Point(4), Size(120), Color(255, 0));

		float angle = 0.f;
		Point pos(100, 300);

		while (!input->IsWillQuit())
		{
			input->Update();
			world.Update(0.5f);

			if (input->IsKeyDown(SDL_SCANCODE_W))
			{
				e->Get<Position>()->pos.y -= 10;
			}
			if (input->IsKeyDown(SDL_SCANCODE_S))
			{
				e->Get<Position>()->pos.y += 10;
			}
			if (input->IsKeyPressed(SDL_SCANCODE_A))
			{
				e->Get<Position>()->pos.x -= 10;
			}
			if (input->IsKeyPressed(SDL_SCANCODE_D))
			{
				e->Get<Position>()->pos.x += 10;
			}

			graphics->Begin();
			graphics->DrawImage(hero, pos, e->Get<Rotation>()->angle);
			graphics->DrawImage(block, e->Get<Position>()->pos);
			graphics->End();
		}

		delete block;
		delete hero;
	
		return 0;
	}
}
