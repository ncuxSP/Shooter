#include "stdafx.h"

#include "Components.h"

#include "Physics.h"

PhysicSystem::~PhysicSystem()
{

}

void PhysicSystem::Configure(World* _world)
{
	_world->Subscribe<Events::OnComponentAssigned<PhysicBody>>(this);
}

void PhysicSystem::UnConfigure(World* _world)
{
	_world->UnSubscribeAll(this);
}

void PhysicSystem::Update(World* _world, float _dt)
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
			auto vs = bullet->Assign<Visual>(new Image("data/bullet.png"));
			auto radius = 6.f;
			bullet->Assign<Translation>(_tr->position + direction * (_pb->radius + radius + 1.f), vs->image->GetSize(), 0.f);
			auto pb = bullet->Assign<PhysicBody>(1.f, radius);
			pb->velocity = direction * 300.f;
			_ct->shoot_delay = 0.1f;
		}
	});

	_world->Each<Translation, PhysicBody>([&](Entity *_entity, ComponentPtr<Translation> _tr, ComponentPtr<PhysicBody> _pb) -> void
	{
		_tr->position += _pb->velocity * _dt;
	});
}

void PhysicSystem::Receive(World *_world, const Events::OnComponentAssigned<PhysicBody> &_event)
{
	auto tr = _event.entity->Get<Translation>();
}

CollisionSystem::CollisionSystem(const Size &_size) : size(_size)
{

}

CollisionSystem::~CollisionSystem()
{

}

void CollisionSystem::Configure(World* _world)
{

}

void CollisionSystem::UnConfigure(World* _world)
{

}

void CollisionSystem::Update(World* _world, float _dt)
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
				else if (abs(normal.y) < size.h / 2.f)
				{
					normal.y = 0.f;
				}
				else
				{
					int a = 9;
					++a;
				}
				auto diff = normal.GetNormalized() * size / 2.f - normal;
				_tr->position += diff * 2.f;
			}
		}
	});
}
