#include "stdafx.h"

#include "Components.h"

#include "Controller.h"

ControllerSystem::ControllerSystem(Input *_input)
	: input(_input)
{

}

ControllerSystem::~ControllerSystem()
{
	for (auto c : controls)
	{
		delete c.second;
	}
}

void ControllerSystem::Configure(World *_world)
{
	_world->Subscribe<EndRound>(this);

	BindControls();
	InitAI(_world);
}

void ControllerSystem::InitAI(World* _world)
{
	ai = BTBuilder()
		.Selector("select behavior")
			.Sequence("retreat")
				.Condition("is too dangerous", [&]() -> bool { return BulletsCount() >= 4; })
				.Action("hiding", bind(&ControllerSystem::Hiding, this))
				.Action("aiming", bind(&ControllerSystem::AimingToLocation, this))
				.Action("moving", bind(&ControllerSystem::Moving, this))
			.End()
			.Sequence("shoot if can")
				.Condition("if seeing target", bind(&ControllerSystem::IsCanSeeTarget, this))
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
				.Action("aiming", bind(&ControllerSystem::AimingToLocation, this))
				.Action("moving", bind(&ControllerSystem::Moving, this))
			.End()
		.End()
		.Build();

	blackboard.world = _world;
	FindNewLocation();
}

void ControllerSystem::BindControls()
{
	auto control = CreateControl<float>("Move", 0.0f);
	control->Bind(SDL_SCANCODE_W, 1.0f);
	control->Bind(SDL_SCANCODE_UP, 1.0f);
	control->Bind(SDL_SCANCODE_S, -1.0f);
	control->Bind(SDL_SCANCODE_DOWN, -1.0f);

	control = CreateControl<float>("Strafe", 0.0f);
	control->Bind(SDL_SCANCODE_E, 1.0f);
	control->Bind(SDL_SCANCODE_X, 1.0f);
	control->Bind(SDL_SCANCODE_Q, -1.0f);
	control->Bind(SDL_SCANCODE_Z, -1.0f);

	control = CreateControl<float>("Rotate", 0.0f);
	control->Bind(SDL_SCANCODE_D, 1.0f);
	control->Bind(SDL_SCANCODE_RIGHT, 1.0f);
	control->Bind(SDL_SCANCODE_A, -1.0f);
	control->Bind(SDL_SCANCODE_LEFT, -1.0f);

	auto shoot = CreateControl<bool>("Shoot", false);
	shoot->Bind(SDL_SCANCODE_RETURN, true);
	shoot->Bind(SDL_SCANCODE_SPACE, true);
}

void ControllerSystem::UnConfigure(World *_world)
{
	_world->UnSubscribeAll(this);
}

void ControllerSystem::Update(World *_world, float _dt)
{
	_world->Each<Controller>([&](Entity *_entity, ComponentPtr<Controller> _ct) -> void
	{
		if (_entity->Is("Hero"))
		{
			_ct->axis.y = GetControlValue<float>("Move");
			_ct->axis.x = GetControlValue<float>("Strafe");
			_ct->axis.Normalize();

			_ct->rotation = GetControlValue<float>("Rotate");
			_ct->is_shooting = GetControlValue<bool>("Shoot");
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

void ControllerSystem::Receive(World *_world, const EndRound &_event)
{
	FindNewLocation();
}

bool ControllerSystem::IsInTargetLocation() const
{
	if (!blackboard.location_found)
	{
		return false;
	}

	auto tr = blackboard.me->Get<Translation>();
	auto pb = blackboard.me->Get<PhysicBody>();

	return (blackboard.location - tr->position).Length() < pb->radius;
}

Engine::BTStatus ControllerSystem::Moving()
{
	auto ct = blackboard.me->Get<Controller>();
	ct->axis.y = 1.f;

	if (IsInTargetLocation() || (IsCanSeeTarget() && BulletsCount() < 4))
	{
		ct->axis.y = 0.f;
		return BTStatus::Success;
	}

	return BTStatus::Running;
}

Engine::BTStatus ControllerSystem::FindNewLocation()
{
	Random rnd;
	Vector location;
	do
	{
		location = Vector(rnd.GetFromZero(1280.f), rnd.GetFromZero(800.f));
	} 
	while (!IsCanSeeTargetFrom(location));

	blackboard.location = location;
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
			}
	);

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

uint32_t ControllerSystem::BulletsCount()
{
	uint32_t result = 0;
	blackboard.world->All([&](Entity *_entity) -> void
	{
		if (_entity->Is("Bullet"))
		{
			++result;
		}
	});
	return result;
}

Engine::BTStatus ControllerSystem::Aiming()
{
	return AimingTo(blackboard.target);
}

Engine::BTStatus ControllerSystem::AimingToLocation()
{
	return AimingTo(blackboard.location);
}

Engine::BTStatus ControllerSystem::AimingTo(const Point &_position)
{
	auto ct = blackboard.me->Get<Controller>();
	auto tr = blackboard.me->Get<Translation>();

	auto pi = acosf(-1);
	auto rad = tr->angle * pi / 180.f;
	Vector direction(cosf(rad), sinf(rad));

	ct->rotation = Math::Sign(Math::DotProduct(direction, -tr->position + _position ));

	if (IsAimed())
	{
		ct->rotation = 0.f;
		return BTStatus::Success;
	}

	return BTStatus::Running;
}

Engine::BTStatus ControllerSystem::Shooting()
{
	auto ct = blackboard.me->Get<Controller>();
	ct->is_shooting = true;
	ct->axis.y = 0.f;
	ct->rotation = 0.f;

	return BTStatus::Success;
}

Engine::BTStatus ControllerSystem::Hiding()
{
	Random rnd;
	Vector location;
	do
	{
		location = Vector(rnd.GetFromZero(1280.f), rnd.GetFromZero(800.f));
	}
	while (IsCanSeeTargetFrom(location));

	blackboard.location = location;
	blackboard.location_found = true;

	return BTStatus::Success;
}
