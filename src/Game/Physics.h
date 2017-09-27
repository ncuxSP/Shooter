#pragma once

class PhysicSystem : public EntitySystem, EventSubscriber<Events::OnComponentAssigned<PhysicBody>>
{
public:
	virtual ~PhysicSystem() override = default;

	virtual void Configure(World* _world) override;

	virtual void UnConfigure(World* _world) override;

	virtual void Update(World* _world, float _dt) override;

	virtual void Receive(World *_world, const Events::OnComponentAssigned<PhysicBody> &_event) override;
};

class CollisionSystem : public EntitySystem
{
public:
	CollisionSystem(const Size &_size);

	virtual ~CollisionSystem() override = default;

	virtual void Configure(World* _world) override;

	virtual void UnConfigure(World* _world) override;

	virtual void Update(World* _world, float _dt) override;

private:
	void ApplyCircleBoxCollision(ComponentPtr<Translation> _circle_tr, ComponentPtr<PhysicBody> _circle_pb, ComponentPtr<Translation> _box_tr);

private:
	Graphics *graphics;
	Size size;
};



