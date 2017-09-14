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
	void SpawnBlocks();

private:
	unique_ptr<World> world;
};