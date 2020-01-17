#pragma once

class PhysicSystem : public EntitySystem, EventSubscriber<Events::OnComponentAssigned<PhysicBody>>
{
public:
	~PhysicSystem() override = default;

	void Configure(World *_world) override;
	void UnConfigure(World *_world) override;
	void Update(World *_world, float _dt) override;
	void Receive(World *_world, const Events::OnComponentAssigned<PhysicBody> &_event) override;
};

class CollisionSystem : public EntitySystem
{
public:
	CollisionSystem(const Size &_size);

	~CollisionSystem() override = default;

	void Configure(World *_world) override;
	void UnConfigure(World *_world) override;
	void Update(World *_world, float _dt) override;

private:
	void ApplyCircleBoxCollision(ComponentPtr<Translation> _circle_tr, ComponentPtr<PhysicBody> _circle_pb, ComponentPtr<Translation> _box_tr);

private:
	Graphics *graphics;
	Size size;
};



