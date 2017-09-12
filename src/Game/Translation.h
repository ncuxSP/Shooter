#pragma once

struct Translation
{
	Point position;
	float rotation;
};

class TranslationSystem : public EntitySystem
{
public:
	virtual ~TranslationSystem();

	virtual void Configure(World* _world);

	virtual void UnConfigure(World* _world);

	virtual void Update(World* _world, float _dt);
};