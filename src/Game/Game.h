#pragma once

class Game : public Engine::Application
{
public:
	Game(const string &_name, const Size &_size);
	virtual ~Game() override;

	virtual void OnBegin() override;
	virtual void OnUpdate(float _dt) override;
	virtual void OnRender() override;
	virtual void OnEnd() override;

private:
	unique_ptr<World> world;
	unique_ptr<Entity> e;

	Image *hero;
	Image *block;
};