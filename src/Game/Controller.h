#pragma once

class ControllerSystem : public EntitySystem
{
public:
	ControllerSystem(Input *_input);

	virtual ~ControllerSystem() override = default;

	virtual void Configure(World *_world) override;

	virtual void UnConfigure(World *_world) override;

	virtual void Update(World *_world, float _dt) override;

private:
	//Behaviors
	BTStatus Moving();
	BTStatus FindNewLocation();
	BTStatus Aiming();
	BTStatus AimingTo(const Point &_position);
	BTStatus Shooting();
	bool IsInTargetLocation() const; 
	bool IsCanSeeTargetFrom(const Point &_position) const;
	bool IsCanSeeTarget() const;
	bool IsAimed() const;

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



