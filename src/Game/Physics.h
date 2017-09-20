#pragma once

class PhysicSystem : public EntitySystem, EventSubscriber<Events::OnComponentAssigned<PhysicBody>>
{
public:
	virtual ~PhysicSystem() override;

	virtual void Configure(World* _world) override;

	virtual void UnConfigure(World* _world) override;

	virtual void Update(World* _world, float _dt) override;

	virtual void Receive(World *_world, const Events::OnComponentAssigned<PhysicBody> &_event) override;
};

class CollisionSystem : public EntitySystem
{
public:
	CollisionSystem(const Size &_size);

	virtual ~CollisionSystem() override;

	virtual void Configure(World* _world) override;

	virtual void UnConfigure(World* _world) override;

	virtual void Update(World* _world, float _dt) override;

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



