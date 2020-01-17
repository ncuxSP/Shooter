#pragma once

struct ScoreText
{
	uint32_t hero;
	uint32_t enemy;
	ImageText *text;
};

struct EndRound;

class Game : public Application, EventSubscriber<EndRound>
{
public:
	Game(const string &_name, const Size &_size);
	~Game() override;

	void OnBegin() override;
	void OnUpdate(float _dt) override;
	void OnRender() override;
	void OnEnd() override;

	void Receive(World *_world, const EndRound &_event) override;

private:
	void SpawnBlocks(uint32_t _count);
	void SpawnCharacters();

private:
	unique_ptr<World> world;
	bool is_game_started;
	float spawn_characters_delay{};
	ScoreText scores{};
	bool restart;
};
