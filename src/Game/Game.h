#pragma once

class Game : public Application
{
public:
	Game(const string &_name, const Size &_size);
	virtual ~Game() override;

	virtual void OnBegin() override;
	virtual void OnUpdate(float _dt) override;
	virtual void OnRender() override;
	virtual void OnEnd() override;

private:
	void SpawnBlocks(uint32_t _count);
	void SpawnCharacters();

private:
	unique_ptr<World> world;
	bool is_game_started;
	float spawn_characters_delay;
};