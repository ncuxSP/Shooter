#include "stdafx.h"

#include "Game.h"

Game::Game(const string &_name, const Size &_size)
	:	Application(_name, _size)
{

}

Game::~Game()
{
	delete hero;
	delete block;
}

void Game::OnBegin()
{
	Application::OnBegin();

	graphics->SetColor(Color(255));

	world = make_unique<World>();
	world->RegisterSystem(new RotationSystem());

	e.reset(world->Create());
	e->Assign<Rotation>(0.f);
	e->Assign<Position>(Point(100, 300));

	hero = graphics->NewImage("hero.bmp");
	block = new Image(Size(128), Color(0, 255));
	block->FillRect(Point(4), Size(120), Color(255, 0));

	float angle = 0.f;
	Point pos(100, 300);
}

void Game::OnUpdate(float _dt)
{
	Application::OnUpdate(_dt);

	world->Update(_dt);

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
}

void Game::OnRender()
{
	Application::OnRender();

	graphics->DrawImage(hero, Point(300, 100), e->Get<Rotation>()->angle);
	graphics->DrawImage(block, e->Get<Position>()->pos);

}

void Game::OnEnd()
{
	Application::OnEnd();
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
			if (_pos->pos.x > 300)
			{
				_pos->pos.x = 100;
			}
		});
	}

	virtual void Receive(World *_world, const Events::OnComponentAssigned<Rotation> &_event) override
	{
		_event.component->angle = 30.f;
	}
};