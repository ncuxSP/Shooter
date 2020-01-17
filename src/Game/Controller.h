#pragma once

class BaseControl {
public:
	virtual ~BaseControl() = 0 {}
};

template <class ValueType>
class Control : public BaseControl {
public:
	using Ptr = shared_ptr<Control<ValueType>>;

public:
	Control(ValueType _default)
		: default_value(_default)
	{}

	~Control() override
	{
		cout << "delete control " << endl;
	}

	void Bind(uint32_t _key, ValueType _value)
	{
		bindings.insert({ _key, _value });
	}

	ValueType Update(Input *_input)
	{
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
	ValueType default_value;
	unordered_map<uint32_t, ValueType> bindings;
};

class ControllerSystem : public EntitySystem, EventSubscriber<EndRound>
{
public:
	ControllerSystem(Input *_input);

	~ControllerSystem() override;

	void Configure(World *_world) override;
	void UnConfigure(World *_world) override;
	void Update(World *_world, float _dt) override;
	void Receive(World *_world, const EndRound &_event) override;

private:
	template <class ValueType>
	shared_ptr<Control<ValueType>> CreateControl(const string &_name, ValueType _default);

	template <class ValueType>
	ValueType GetControlValue(const string &_name);
	void BindControls();

	void InitAI(World *_world);

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

	unordered_map<string, shared_ptr<BaseControl>> controls;

	BTree ai;
	struct
	{
		World *world{};
		Entity *me{};
		float dt{};
		Vector target;
		bool location_found{};
		Vector location;
	} blackboard;
};

template <class ValueType>
shared_ptr<Control<ValueType>> ControllerSystem::CreateControl(const string &_name, ValueType _default)
{
	auto c = make_shared<Control<ValueType>>(_default);
	controls.insert({ _name, c });
	return c;
}

template <class ValueType>
ValueType ControllerSystem::GetControlValue(const string &_name)
{
	if (controls.count(_name) == 0)
	{
		cout << "Can't find control with name " << _name << endl;
	}
	auto control = reinterpret_cast<Control<ValueType> *>(controls[_name].get());
	return control->Update(input);
}

