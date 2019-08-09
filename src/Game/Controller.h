#pragma once

class BaseControl {
public:
	virtual ~BaseControl() = 0 {}
};

template <class T>
class Control : public BaseControl {
public:
	Control(T _default)
		: default_value(_default)
	{}

	virtual ~Control() override {}

	void Bind(uint32_t _key, T _value) {
		bindings.insert({ _key, _value });
	}

	T Update(Input* _input) {
		for (auto c : bindings)
		{
			if (_input->IsKeyPressed(c.first))
			{
				return c.second;
			}
		}
		return default_value;
	}

private:
	T default_value;
	unordered_map<uint32_t, T> bindings;
};

class ControllerSystem : public EntitySystem, EventSubscriber<EndRound>
{
public:
	ControllerSystem(Input *_input);

	virtual ~ControllerSystem() override;

	virtual void Configure(World *_world) override;

	virtual void UnConfigure(World *_world) override;

	virtual void Update(World *_world, float _dt) override;

	virtual void Receive(World *_world, const EndRound &_event) override;

private:
	template <class T>
	Control<T> *CreateControl(const string &_name, T _value);

	template <class T>
	T GetControlValue(const string& _name);

	void InitAI(World* _world);

	void BindControls();

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

	unordered_map<string, BaseControl *> controls;
	
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

template <class T>
Control<T> *ControllerSystem::CreateControl(const string& _name, T _default)
{
	auto c = new Control<T>(_default);
	controls.insert({ _name, c });
	return c;
}

template <class T>
T ControllerSystem::GetControlValue(const string& _name)
{
	if (controls.count(_name) == 0)
	{
		cout << "Can't find control with name " << _name << endl;
	}
	auto control = reinterpret_cast<Control<T> *>(controls[_name]);
	return control->Update(input);
}

