#include "stdafx.h"

#include "Game.h"

Game::Game(const string &_name, const Size &_size)
	:	Application(_name, _size)
	,	is_game_started(false)
{

}

Game::~Game()
{
}

struct Translation
{
	Translation(const Vector &_position, const Size &_size, float _angle)
		:	position(_position)
		,	size(_size)
		,	angle(_angle)
	{}

	Size size;
	Vector position;
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

	Controller(Type _type) : type(_type), axis(0.f), rotation(0.f), is_shooting(false) {}

	Vector axis;
	float rotation;
	bool is_shooting;
	float shoot_delay;
};

struct PhysicBody
{
	PhysicBody(float _mass, float _radius)
		:	is_circle(true)
		,	mass(_mass)
		,	radius(_radius)
		,	velocity(0.f)
	{
	}

	PhysicBody(float _mass, const Translation &_tr)
		:	is_circle(false)
		,	mass(_mass)
		,	velocity(0.f)
	{
		radius = _tr.size.Length() / 2.f;
	}
	
	bool is_circle;
	float radius;
	float mass;

	Vector velocity;
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

			auto pi = acosf(-1);
			auto rad = _tr->angle * pi / 180.f;
			Vector direction(sinf(rad), -cosf(rad));

			_pb->velocity = direction * _ct->axis.y + Vector(-direction.y, direction.x) * _ct->axis.x;
			_pb->velocity *= 80.f;

			_ct->shoot_delay -= _dt;

			if (_ct->is_shooting && _ct->shoot_delay < 0.f)
			{
				auto bullet = _world->Create("Bullet");
				auto vs = bullet->Assign<Visual>(new Image("bullet.bmp"));
				bullet->Assign<Translation>(_tr->position + direction * _pb->radius, vs->image->GetSize(), 0.f);
				auto pb = bullet->Assign<PhysicBody>(1.f, 6.f);
				pb->velocity = direction * 300.f;
				_ct->shoot_delay = 0.1f;
			}
		});

		_world->Each<Translation, PhysicBody>([&](Entity *_entity, ComponentPtr<Translation> _tr, ComponentPtr<PhysicBody> _pb) -> void
		{
			_tr->position += _pb->velocity * _dt;
		});
	}

	virtual void Receive(World *_world, const Events::OnComponentAssigned<PhysicBody> &_event) override
	{
		auto tr = _event.entity->Get<Translation>();
	}
};

class CollisionSystem : public EntitySystem
{
public:
	CollisionSystem(const Size &_size)
		:	size(_size)
	{}

	virtual ~CollisionSystem() override {}

	virtual void Configure(World* _world) override
	{
	}

	virtual void UnConfigure(World* _world) override
	{
	}

	virtual void Update(World* _world, float _dt) override
	{
		list<Entity *> candidates;

		_world->Each<Translation, PhysicBody>([&](Entity *_entity, ComponentPtr<Translation> _tr, ComponentPtr<PhysicBody> _pb) -> void
		{
			candidates.push_back(_entity);
		});

		_world->Each<Translation, PhysicBody>([&](Entity *_entity, ComponentPtr<Translation> _tr, ComponentPtr<PhysicBody> _pb) -> void
		{
			candidates.remove(_entity);

			for (auto e : candidates)
			{
				auto tr = e->Get<Translation>();
				auto pb = e->Get<PhysicBody>();

				if ((tr->position - _tr->position).Length() <= pb->radius + _pb->radius)
				{
					if (pb->is_circle && _pb->is_circle)
					{
						auto rv = pb->velocity - _pb->velocity;
						auto normal = tr->position - _tr->position;
						normal.Normalize();

						float contact = DotProduct(rv, normal);
						if (contact > 0)
						{
							continue;
						}

						float j = -2 * contact / (pb->mass + _pb->mass);
						auto impulse = normal * j;

						pb->velocity += impulse * _pb->mass;
						_pb->velocity -= impulse * pb->mass;
					}
					else if (!pb->is_circle && !_pb->is_circle)
					{
						auto normal = tr->position - _tr->position;
						normal.SetLength((pb->radius + _pb->radius - normal.Length()) / 2.f + 1.f);
						if (normal.x < 0)
						{
							normal = -normal;
						}
						if (normal.y < 0)
						{
							normal = -normal;
						}

						normal *= 1.f / (pb->mass + _pb->mass);

						tr->position -= normal * _pb->mass;
						_tr->position += normal * pb->mass;

						continue;
					}
					else if (!pb->is_circle && _pb->is_circle)
					{
						_pb->velocity = -_pb->velocity;
					}
					else if (pb->is_circle && !_pb->is_circle)
					{
						if (_entity->Is("SpawnHero") || _entity->Is("SpawnEnemy"))
						{
							continue;
						}

						auto circle_pos = Rotate(tr->position - _tr->position, _tr->angle);
						auto closest_point = circle_pos;

						if (circle_pos.x < _tr->size.w / -2.f)
						{
							closest_point.x = _tr->size.w / -2.f;
						}
						else if (circle_pos.x > _tr->size.w / 2.f)
						{
							closest_point.x = _tr->size.w / 2.f;
						}
						if (circle_pos.y < _tr->size.h / -2.f)
						{
							closest_point.y = _tr->size.h / -2.f;
						}
						else if (circle_pos.y > _tr->size.h / 2.f)
						{
							closest_point.y = _tr->size.h / 2.f;
						}

						auto normal = circle_pos - closest_point;

						if (normal.Length() < pb->radius)
						{
							if (normal.Length() < numeric_limits<float>::epsilon())
							{
								auto diff = _tr->size / 2.f - Vector(abs(circle_pos.x), abs(circle_pos.y));
								if (diff.x < diff.y)
								{
									normal.x = circle_pos.x / abs(circle_pos.x);
								}
								else
								{
									normal.y = circle_pos.y / abs(circle_pos.y);
								}
								tr->position += Rotate(normal * diff * 2.f, -_tr->angle);
							}
							else
							{
								auto diff = normal.GetNormalized() * (pb->radius - normal.Length());
								tr->position += Rotate(diff * 2.f, -_tr->angle);
							}

							auto velocity = Rotate(pb->velocity, _tr->angle);
							normal.Normalize();
							velocity -= normal * 2.f * DotProduct(velocity, normal);
							pb->velocity = Rotate(velocity, -_tr->angle);
						}
					}

				}
			}
		});

		_world->Each<Translation>([&](Entity *_entity, ComponentPtr<Translation> _tr) -> void 
		{
			if (_tr->position.x < 0.f || _tr->position.x > size.w || _tr->position.y < 0.f || _tr->position.y > size.h)
			{
				if (_entity->Is("Bullet"))
				{
					_world->Destroy(_entity);
				}
				else
				{
					auto normal = _tr->position - size / 2.f;
					if (abs(normal.x) < size.w / 2.f)
					{
						normal.x = 0.f;
					}
					if (abs(normal.y) < size.h / 2.f)
					{
						normal.y = 0.f;
					}
					auto diff = normal.GetNormalized() * size / 2.f - normal;
					_tr->position += diff * 2.f;
				}
			}
		});
	}

	Vector Rotate(const Vector &_v, float _angle)
	{
		auto pi = acosf(-1);
		auto rad = -_angle * pi / 180.f;
		return Vector(_v.x * cosf(rad) - _v.y * sinf(rad), _v.x * sinf(rad) + _v.y * cosf(rad));
	}

private:
	Graphics *graphics;
	Size size;
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
		auto visual = hero->Assign<Visual>(new Image("hero.bmp"));
		hero->Assign<Translation>(_entity->Get<Translation>()->position, visual->image->GetSize(), 0.f);
		hero->Assign<Controller>(Controller::Type::Player);
		hero->Assign<PhysicBody>(10.f, 40.f);
	});

	world->EachTag("SpawnEnemy", [&](Entity *_entity) -> void
	{
		auto enemy = world->Create("Enemy");
		auto visual = enemy->Assign<Visual>(new Image("hero.bmp"));
		enemy->Assign<Translation>(_entity->Get<Translation>()->position, visual->image->GetSize(), 180.f);
		enemy->Assign<Controller>(Controller::Type::Player);
		enemy->Assign<PhysicBody>(10.f, 40.f);
	});

}
