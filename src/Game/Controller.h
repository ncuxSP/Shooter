#pragma once

class ControllerSystem : public EntitySystem, EventSubscriber<EndRound>
{
public:
	ControllerSystem(Input *_input);

	virtual ~ControllerSystem() override = default;

	virtual void Configure(World *_world) override;

	virtual void UnConfigure(World *_world) override;

	virtual void Update(World *_world, float _dt) override;

	virtual void Receive(World *_world, const EndRound &_event) override;
private:
	//Behaviors
	BTStatus Moving();
	BTStatus FindNewLocation();
	BTStatus Aiming();
	BTStatus AimingToLocation();
	BTStatus AimingTo(const Point &_position);
	BTStatus Shooting();
	BTStatus Hiding();
	bool IsInTargetLocation() const; 
	bool IsCanSeeTargetFrom(const Point &_position) const;
	bool IsCanSeeTarget() const;
	bool IsAimed() const;
	uint32_t BulletsCount();

private:
	Input *input;
	
	BTree ai;
	struct
	{
		World *world;
		Entity *me;
		float dt;
		Vector target;
		bool location_found;
		Vector location;
	} blackboard;
};



