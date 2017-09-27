#include "stdafx.h"

#include "Components.h"
#include "Physics.h"
#include "Controller.h"

#include "Game.h"


Game::Game(const string &_name, const Size &_size)
	:	Application(_name, _size)
	,	is_game_started(false)
	,	restart(false)
{
}

Game::~Game() = default;

void Game::OnBegin()
{
	Application::OnBegin();

	graphics->SetColor(Color(255));

	world = make_unique<World>();
	world->RegisterSystem(new CollisionSystem(size));
	world->RegisterSystem(new PhysicSystem());
	world->RegisterSystem(new ControllerSystem(input));

	world->Subscribe<EndRound>(this);

	SpawnBlocks(8);

	scores.hero = 0;
	scores.enemy = 0;
	scores.text = new ImageText("0:0", Size(128, 48), Color(0, 255));
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

	if (restart)
	{
		SpawnBlocks(8);
		restart = false;
		is_game_started = false;
	}
}

void Game::OnRender()
{
	Application::OnRender();

	world->Each<Visual, Translation>([&](Entity *_entity, ComponentPtr<Visual> _vs, ComponentPtr<Translation> _tr) -> void
	{
		graphics->DrawImage(_vs->image.get(), _tr->position - _tr->size / 2, _tr->angle);
	});

	graphics->DrawImage(scores.text, Point(0));
}

void Game::OnEnd()
{
	Application::OnEnd();

	delete scores.text;

	world->UnSubscribeAll(this);
}

void Game::Receive(World *_world, const EndRound &_event)
{
	if (_event.loser->Is("Hero"))
	{
		++scores.enemy;
	}
	else
	{
		++scores.hero;
	}

	scores.text->SetText(to_string(scores.hero) + ":" + to_string(scores.enemy));

	world->Reset();
	restart = true;
}

void Game::SpawnBlocks(uint32_t _count)
{
	Random rnd;

	for (uint32_t i = 0; i < _count; ++i)
	{
		auto box = world->Create("Box");

		Size size(rnd.Get(4, 16) * 16, rnd.Get(4, 16) * 16);
		auto image = new Image(size, Color(0, 255));
		image->FillRect(Point(4), size - Size(8), Color(255, 0));
		box->Assign<Visual>(image);

		Point pos(rnd.Get(15, 65) * 16, rnd.Get(15, 35) * 16);
		float angle = rnd.GetFromZero(36) * 10.f;
		auto tr = box->Assign<Translation>(pos, size, angle);

		box->Assign<PhysicBody>(10.f, *tr.Get());
	}

	auto hero_spawn = world->Create("SpawnHero");
	Point position(640, 700);
	auto tr = hero_spawn->Assign<Translation>(position, Size(128), 0.f);
	hero_spawn->Assign<PhysicBody>(1000000.f, *tr.Get());

	auto enemy_spawn = world->Create("SpawnEnemy");
	position = Point(640, 100);
	tr = enemy_spawn->Assign<Translation>(position, Size(128), 0.f);
	enemy_spawn->Assign<PhysicBody>(1000000.f, *tr.Get());

	spawn_characters_delay = 0.5f;
}

void Game::SpawnCharacters()
{
	auto spawn = world->GetByTag("SpawnHero");

	auto hero = world->Create("Hero");
	auto visual = hero->Assign<Visual>(new Image("data/hero.png"));
	hero->Assign<Translation>(spawn->Get<Translation>()->position, visual->image->GetSize(), 0.f);
	hero->Assign<Controller>();
	hero->Assign<PhysicBody>(10.f, 32.f);

	spawn = world->GetByTag("SpawnEnemy");
	
	auto enemy = world->Create("Enemy");
	visual = enemy->Assign<Visual>(new Image("data/hero.png"));
	enemy->Assign<Translation>(spawn->Get<Translation>()->position, visual->image->GetSize(), 180.f);
	enemy->Assign<Controller>();
	enemy->Assign<PhysicBody>(10.f, 32.f);

}
