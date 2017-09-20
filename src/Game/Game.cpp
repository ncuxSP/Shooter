#include "stdafx.h"

#include "Components.h"
#include "Physics.h"

#include "Game.h"

Game::Game(const string &_name, const Size &_size)
	:	Application(_name, _size)
	,	is_game_started(false)
{
}

Game::~Game() = default;

class ControllerSystem : public EntitySystem
{
public:
	ControllerSystem(Input *_input) : input(_input) {}

	virtual ~ControllerSystem() override = default;

	virtual void Configure(World* _world) override
	{
		ai = BehaviorTree::Builder()
			.Inverter("invert something")
				.Action("do something", []() -> BehaviorTree::Status 
					{
						return BehaviorTree::Status::Success;
					})
			.End()
			.Build();
	}

	virtual void UnConfigure(World* _world) override
	{
	}

	virtual void Update(World* _world, float _dt) override
	{
		_world->Each<Controller>([&](Entity *_entity, ComponentPtr<Controller> _ct) -> void
		{
			if (_ct->type == Controller::Type::Player)
			{
				if (input->IsKeyPressed(SDL_SCANCODE_W))
				{
					_ct->axis.y = 1.f;
				}
				else if (input->IsKeyPressed(SDL_SCANCODE_S))
				{
					_ct->axis.y = -1.f;
				}
				else
				{
					_ct->axis.y = 0.f;
				}

				if (input->IsKeyPressed(SDL_SCANCODE_Q))
				{
					_ct->axis.x = -1.f;
				}
				else if (input->IsKeyPressed(SDL_SCANCODE_E))
				{
					_ct->axis.x = 1.f;
				}
				else
				{
					_ct->axis.x = 0.f;
				}

				_ct->axis.Normalize();

				if (input->IsKeyPressed(SDL_SCANCODE_A))
				{
					_ct->rotation = -1.f;
				}
				else if (input->IsKeyPressed(SDL_SCANCODE_D))
				{
					_ct->rotation = 1.f;
				}
				else
				{
					_ct->rotation = 0.f;
				}

				if (input->IsKeyDown(SDL_SCANCODE_RETURN) || input->IsKeyDown(SDL_SCANCODE_SPACE))
				{
					_ct->is_shooting = true;
				}
				else
				{
					_ct->is_shooting = false;
				}
			}
		});
	}

private:
	Input *input;
	BehaviorTree::Node::Ptr ai;
};


void Game::OnBegin()
{
	Application::OnBegin();

	graphics->SetColor(Color(255));

	world = make_unique<World>();
	world->RegisterSystem(new CollisionSystem(size));
	world->RegisterSystem(new PhysicSystem());
	world->RegisterSystem(new ControllerSystem(input));

	SpawnBlocks(8);

	auto t = world->Create("Text");
	t->Assign<Text>(new ImageText("0:0", Size(128, 48), Color(0, 255)));
	t->Assign<Translation>(Point(64, 24), Size(128, 48), 0.f);
}

void Game::OnUpdate(float _dt)
{
	Application::OnUpdate(_dt);

	world->Update(_dt);

	if (!is_game_started && spawn_characters_delay < 0.f)
	{
		SpawnCharacters();
		is_game_started = true;
	}
	spawn_characters_delay -= _dt;
}

void Game::OnRender()
{
	Application::OnRender();

	world->Each<Visual, Translation>([&](Entity *_entity, ComponentPtr<Visual> _vs, ComponentPtr<Translation> _tr) -> void
	{
		graphics->DrawImage(_vs->image.get(), _tr->position - _tr->size / 2, _tr->angle);
	});

	world->Each<Text, Translation>([&](Entity *_entity, ComponentPtr<Text> _vs, ComponentPtr<Translation> _tr) -> void
	{
		graphics->DrawImage(_vs->image.get(), _tr->position - _tr->size / 2, _tr->angle);
	});
}

void Game::OnEnd()
{
	Application::OnEnd();
}

void Game::SpawnBlocks(uint32_t _count)
{
	Random rnd;

	for (uint32_t i = 0; i < _count; ++i)
	{
		auto box = world->Create();

		Size size(rnd.Get(4, 16) * 16, rnd.Get(4, 16) * 16);
		auto image = new Image(size, Color(0, 255));
		image->FillRect(Point(4), size - Size(8), Color(255, 0));
		box->Assign<Visual>(image);

		Point pos(rnd.Get(15, 65) * 16, rnd.Get(15, 35) * 16);
		float angle = rnd.GetFromZero(36) * 10.f;
		auto tr = box->Assign<Translation>(pos, size, angle);

		box->Assign<PhysicBody>(10.f, tr.Get());
	}

	auto hero_spawn = world->Create("SpawnHero");
	Point position(640, 700);
	auto tr = hero_spawn->Assign<Translation>(position, Size(128), 0.f);
	hero_spawn->Assign<PhysicBody>(1000000.f, tr.Get());

	auto enemy_spawn = world->Create("SpawnEnemy");
	position = Point(640, 100);
	tr = enemy_spawn->Assign<Translation>(position, Size(128), 0.f);
	enemy_spawn->Assign<PhysicBody>(1000000.f, tr.Get());

	spawn_characters_delay = 0.5f;
}

void Game::SpawnCharacters()
{
	world->EachTag("SpawnHero", [&](Entity *_entity) -> void
	{
		auto hero = world->Create("Hero");
		auto visual = hero->Assign<Visual>(new Image("data/hero.png"));
		hero->Assign<Translation>(_entity->Get<Translation>()->position, visual->image->GetSize(), 0.f);
		hero->Assign<Controller>(Controller::Type::Player);
		hero->Assign<PhysicBody>(10.f, 32.f);
	});

	world->EachTag("SpawnEnemy", [&](Entity *_entity) -> void
	{
		auto enemy = world->Create("Enemy");
		auto visual = enemy->Assign<Visual>(new Image("data/hero.png"));
		enemy->Assign<Translation>(_entity->Get<Translation>()->position, visual->image->GetSize(), 180.f);
		enemy->Assign<Controller>(Controller::Type::Player);
		enemy->Assign<PhysicBody>(10.f, 32.f);
	});

}
