#include "stdafx.h"

#include "Components.h"

#include "Controller.h"

ControllerSystem::ControllerSystem(Input *_input)
	: input(_input)
{

}

void ControllerSystem::Configure(World *_world)
{
	ai = BTBuilder()
		.Selector("select behavior")
			.Sequence("shoot if can")
				.Condition("if seing target", bind(&ControllerSystem::IsCanSeeTarget, this))
				.Selector("aiming")
					.Condition("if aimed", bind(&ControllerSystem::IsAimed, this))
					.Action("aiming to target", bind(&ControllerSystem::Aiming, this))
				.End()
				.Action("shooting", bind(&ControllerSystem::Shooting, this))
			.End()
			.Sequence("change location")
				.Selector("find location")
					.Inverter("if not")
						.Condition("in target location", bind(&ControllerSystem::IsInTargetLocation, this))
					.End()
					.Action("find new location", bind(&ControllerSystem::FindNewLocation, this))
				.End()
				.Action("aiming", bind(&ControllerSystem::Aiming, this))
				.Action("moving", bind(&ControllerSystem::Moving, this))
			.End()
		.End()
		.Build();

	blackboard.world = _world;
	blackboard.location_found = false;
}

void ControllerSystem::UnConfigure(World *_world)
{

}

void ControllerSystem::Update(World *_world, float _dt)
{
	_world->Each<Controller>([&](Entity *_entity, ComponentPtr<Controller> _ct) -> void
	{
		if (_entity->Is("Hero"))
		{
			if (input->IsKeyPressed(SDL_SCANCODE_W) || input->IsKeyPressed(SDL_SCANCODE_UP))
			{
				_ct->axis.y = 1.f;
			}
			else if (input->IsKeyPressed(SDL_SCANCODE_S) || input->IsKeyPressed(SDL_SCANCODE_DOWN))
			{
				_ct->axis.y = -1.f;
			}
			else
			{
				_ct->axis.y = 0.f;
			}

			if (input->IsKeyPressed(SDL_SCANCODE_Q) || input->IsKeyPressed(SDL_SCANCODE_Z))
			{
				_ct->axis.x = -1.f;
			}
			else if (input->IsKeyPressed(SDL_SCANCODE_E) || input->IsKeyPressed(SDL_SCANCODE_X))
			{
				_ct->axis.x = 1.f;
			}
			else
			{
				_ct->axis.x = 0.f;
			}

			_ct->axis.Normalize();

			if (input->IsKeyPressed(SDL_SCANCODE_A) || input->IsKeyPressed(SDL_SCANCODE_LEFT))
			{
				_ct->rotation = -1.f;
			}
			else if (input->IsKeyPressed(SDL_SCANCODE_D) || input->IsKeyPressed(SDL_SCANCODE_RIGHT))
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
		else
		{
			blackboard.me = _entity;
			blackboard.dt = _dt;
			blackboard.target = blackboard.world->GetByTag("Hero")->Get<Translation>()->position;

			ai->Update();
		}
	});
}

bool ControllerSystem::IsInTargetLocation() const
{
	if (!blackboard.location_found)
	{
		return true;
	}

	auto tr = blackboard.me->Get<Translation>();
	auto pb = blackboard.me->Get<PhysicBody>();

	return (blackboard.location - tr->position).Length() < pb->radius;
}

Engine::BTStatus ControllerSystem::Moving()
{
	auto ct = blackboard.me->Get<Controller>();
	ct->axis.y = 1.f;

	if (IsInTargetLocation() || IsCanSeeTarget())
	{
		ct->axis.y = 0.f;
		return BTStatus::Success;
	}

	return BTStatus::Running;
}

Engine::BTStatus ControllerSystem::FindNewLocation()
{
	blackboard.location = blackboard.target;
	blackboard.location_found = true;

	return BTStatus::Success;
}

bool ControllerSystem::IsCanSeeTargetFrom(const Point &_position) const
{
	bool result = true;

	blackboard.world->Each<Translation, PhysicBody>(
		[&](Entity *_entity, ComponentPtr<Translation> _tr, ComponentPtr<PhysicBody> _pb) -> void
	{
		if (result && _entity->Is("Box"))
		{
			result = !Math::IntersectSegmentRectangle(_position, blackboard.target - _position, _tr->ToRectangle());
		}
	});

	return result;
}

bool ControllerSystem::IsCanSeeTarget() const
{
	auto position = blackboard.me->Get<Translation>()->position;
	return IsCanSeeTargetFrom(position);
}

bool ControllerSystem::IsAimed() const
{
	auto tr = blackboard.me->Get<Translation>();
	auto radius = blackboard.world->GetByTag("Hero")->Get<PhysicBody>()->radius;

	auto pi = acosf(-1);
	auto rad = tr->angle * pi / 180.f;
	Vector direction(sinf(rad), -cosf(rad));

	return Math::IntersectRayCircle(tr->position, direction, blackboard.target, radius);
}

Engine::BTStatus ControllerSystem::Aiming()
{
	auto ct = blackboard.me->Get<Controller>();
	auto tr = blackboard.me->Get<Translation>();

	auto pi = acosf(-1);
	auto rad = tr->angle * pi / 180.f;
	Vector direction(cosf(rad), sinf(rad));

	ct->rotation = Math::Sign(Math::DotProduct(direction, blackboard.target - tr->position));

	if (IsAimed())
	{
		ct->rotation = 0.f;
		return BTStatus::Success;
	}

	return BTStatus::Running;
}

Engine::BTStatus ControllerSystem::AimingTo(const Point &_position)
{

}

Engine::BTStatus ControllerSystem::Shooting()
{
	auto ct = blackboard.me->Get<Controller>();
	ct->is_shooting = true;
	ct->axis.y = 0.f;
	ct->rotation = 0.f;

	return BTStatus::Success;
}
