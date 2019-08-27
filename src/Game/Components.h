#pragma once


//Components

struct Translation
{
	Translation(const Vector &_position, const Size &_size, float _angle)
		: position(_position)
		, size(_size)
		, angle(_angle)
	{}

	Size size;
	Vector position;
	float angle;

	Math::Rectangle ToRectangle() const
	{
		return { position, size, angle };
	}
};

struct PhysicBody
{
	PhysicBody(float _mass, float _radius)
		: is_circle(true)
		, mass(_mass)
		, radius(_radius)
		, velocity(0.f)
	{}

	PhysicBody(float _mass, const Translation &_tr)
		: is_circle(false)
		, mass(_mass)
		, velocity(0.f)
	{
		radius = _tr.size.Length() / 2.f;
	}

	bool is_circle;
	float radius;
	float mass;

	Vector velocity;
};

struct Visual
{
	Visual(Image *_image) : image(_image) {}

	shared_ptr<Image> image;
};

struct Text
{
	Text(ImageText *_image) : image(_image) {}

	shared_ptr<ImageText> image;
};

struct Controller
{
	Controller()
		: axis(0.f)
		, rotation(0.f)
		, is_shooting(false)
		, shoot_delay(0.f)
	{}

	Vector axis;
	float rotation;
	bool is_shooting;
	float shoot_delay;
};


//Events

struct OnRenderBegin
{
	OnRenderBegin() = default;
};

struct EndRound
{
	EndRound(Entity *_loser)
		: loser(_loser)
	{}

	Entity *loser;
};