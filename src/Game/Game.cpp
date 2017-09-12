#include "stdafx.h"

#include "Game.h"

Game::Game(const string &_name, const Size &_size)
	:	Application(_name, _size)
{

}

Game::~Game()
{
}

struct Translation
{
	Translation(const Point &_position, float _angle) : position(_position), angle(_angle) {}

	Point position;
	float angle;
};

struct Visual
{
	Visual(Image *_image) : image(_image) {}

	shared_ptr<Image> image;
};

struct Controller
{
	enum class Type : uint8_t
	{
		Player,
		AI
	} type;

	Controller(Type _type) : type(_type), axis(0.f), rotation(0.f) {}

	Vector axis;
	float rotation;
	bool is_shooting;
};

struct PhysicBody
{
	Vector position;
	Vector direction;
	Vector velocity;
	float rotation;
};

struct OnRenderBegin
{
	OnRenderBegin() {};
};

class ControllerSystem : public EntitySystem
{
public:
	ControllerSystem(Input *_input) : input(_input) {}
	virtual ~ControllerSystem() override {}

	virtual void Configure(World* _world) override
	{
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

				if (input->IsKeyDown(SDL_SCANCODE_KP_ENTER) || input->IsKeyDown(SDL_SCANCODE_KP_SPACE))
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
};

class PhysicSystem : public EntitySystem, EventSubscriber<Events::OnComponentAssigned<PhysicBody>>
{
public:
	virtual ~PhysicSystem() override {}

	virtual void Configure(World* _world) override
	{
		_world->Subscribe<Events::OnComponentAssigned<PhysicBody>>(this);
	}

	virtual void UnConfigure(World* _world) override
	{
		_world->UnSubscribeAll(this);
	}

	virtual void Update(World* _world, float _dt) override
	{
		_world->Each<Translation, PhysicBody, Controller>(
			[&](Entity *_entity, ComponentPtr<Translation> _tr, ComponentPtr<PhysicBody> _pb, ComponentPtr<Controller> _ct) -> void
		{
			_tr->angle += _ct->rotation * 40.f * _dt;

			auto pi = acos(-1);
			auto rad = _tr->angle * pi / 180.f;
			Vector direction(sin(rad), -cos(rad));
			direction *= 60.f;

			_pb->velocity = direction * _ct->axis.y + Vector(-direction.y, direction.x) * _ct->axis.x;

			if (_ct->is_shooting)
			{
				auto bullet = _world->Create();
				bullet->Assign<Translation>(_tr->position, 0.f);
				bullet->Assign<Visual>(new Image("bullet.bmp"));
				auto pb = bullet->Assign<PhysicBody>();
				pb->velocity = direction * 5.f;
			}
		});

		_world->Each<Translation, PhysicBody>([&](Entity *_entity, ComponentPtr<Translation> _tr, ComponentPtr<PhysicBody> _pb) -> void
		{
			_pb->position += _pb->velocity * _dt;

			_tr->position.x = static_cast<uint32_t>(_pb->position.x);
			_tr->position.y = static_cast<uint32_t>(_pb->position.y);
		});
	}

	virtual void Receive(World *_world, const Events::OnComponentAssigned<PhysicBody> &_event) override
	{
		auto tr = _event.entity->Get<Translation>();
		_event.component->position = tr->position;
	}
};

class VisualSystem : public EntitySystem, EventSubscriber<OnRenderBegin>
{
public:
	VisualSystem(Graphics *_graphics) : graphics(_graphics) {};
	virtual ~VisualSystem() override {}

	virtual void Configure(World* _world) override
	{
		_world->Subscribe<OnRenderBegin>(this);
	}

	virtual void UnConfigure(World* _world) override
	{
		_world->UnSubscribeAll(this);
	}

	virtual void Update(World* _world, float _dt) override
	{
	}

	virtual void Receive(World *_world, const OnRenderBegin &_event) override
	{
		_world->Each<Visual, Translation>([&](Entity *_entity, ComponentPtr<Visual> _vs, ComponentPtr<Translation> _tr) -> void
		{
			graphics->DrawImage(_vs->image.get(), _tr->position, _tr->angle);
		});
	}

private:
	Graphics *graphics;
};

void Game::OnBegin()
{
	Application::OnBegin();

	graphics->SetColor(Color(255));

	world = make_unique<World>();
	world->RegisterSystem(new VisualSystem(graphics));
	world->RegisterSystem(new ControllerSystem(input));
	world->RegisterSystem(new PhysicSystem());

	Random rnd(1, 16);

	for (uint32_t i = 0; i < 5; ++i)
	{
		auto box = world->Create();

		Size size(rnd.Next() * 16);
		auto image = new Image(size, Color(0, 255));
		image->FillRect(Point(4), size - Size(8), Color(255, 0));
		box->Assign<Visual>(image);

		Point pos(rnd.Next() * 64, rnd.Next() * 48);
		float angle = rnd.Next() / 16.f * 360.f;
		box->Assign<Translation>(pos, angle);
	}

	auto hero = world->Create();
	hero->Assign<Visual>(new Image("hero.bmp"));
	hero->Assign<Translation>(Point(500, 500), 0.f);
	hero->Assign<Controller>(Controller::Type::Player);
	hero->Assign<PhysicBody>();
}

void Game::OnUpdate(float _dt)
{
	Application::OnUpdate(_dt);

	world->Update(_dt);
}

void Game::OnRender()
{
	Application::OnRender();

	world->Emit<OnRenderBegin>({});
}

void Game::OnEnd()
{
	Application::OnEnd();
}
