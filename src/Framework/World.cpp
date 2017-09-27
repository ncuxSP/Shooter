#include "stdafx.h"

namespace Engine
{
	World::World()
		: last_id(0)
		, start_reset(false)
	{
	
	}
	
	World::~World()
	{
		for (auto *system : systems)
		{
			system->UnConfigure(this);
			delete system;
		}
		for (auto *entity : entities)
		{
			Emit<Events::OnEntityRemoved>({ entity });
			delete entity;
		}
	}
	
	Entity *World::Create(const string &_tag /*= ""*/)
	{
		++last_id;
		auto entity = new Entity(this, last_id, _tag);
		new_entities.push_back(entity);

		Emit<Events::OnEntityCreated>({ entity });

		return entity;
	}
	
	void World::Destroy(Entity *_entity)
	{
		if (find(removed_entities.begin(), removed_entities.end(), _entity) == removed_entities.end())
		{
			removed_entities.push_back(_entity);

			Emit<Events::OnEntityRemoved>({ _entity });
		}
	}
	
	void World::Update(float _dt)
	{
		for (auto *system : systems)
		{
			system->Update(this, _dt);
		}

		if (removed_entities.size() > 0)
		{
			for (auto *e : removed_entities)
			{
				entities.erase(remove(entities.begin(), entities.end(), e), entities.end());
				delete e;
				e = nullptr;
			}
			removed_entities.clear();
		}

		if (new_entities.size() > 0)
		{
			entities.insert(entities.end(), new_entities.begin(), new_entities.end());
			new_entities.clear();
		}

		if (start_reset)
		{
			for (auto *entity : entities)
			{
				Emit<Events::OnEntityRemoved>({ entity });
				delete entity;
			}
			entities.clear();
			last_id = 0;
			start_reset = false;
		}
	}
	
	void World::Reset()
	{
		start_reset = true;
	}
	
	void World::RegisterSystem(EntitySystem *_system)
	{
		systems.push_back(_system);
		_system->Configure(this);
	}
	
	void World::UnRegisterSystem(EntitySystem *_system)
	{
		systems.erase(remove(systems.begin(), systems.end(), _system), systems.end());
		_system->UnConfigure(this);
	}
	
	void World::UnSubscribeAll(Internal::BaseSubscriber *_subscriber)
	{
		vector<Internal::TypeIndex> to_delete;
		for (auto &pair : subscribers)
		{
			pair.second.erase(remove(pair.second.begin(), pair.second.end(), _subscriber), pair.second.end());
			if (pair.second.size() == 0)
			{
				to_delete.push_back(pair.first);
			}
		}
		for (auto id : to_delete)
		{
			subscribers.erase(id);
		}
		
	}
	
	void World::EachTag(const string &_tag, function<void(Entity *)> _call_back)
	{
		for (auto *entity : entities)
		{
			if (entity->Is(_tag))
			{
				_call_back(entity);
			}
		}
	}

	void World::All(function<void(Entity *)> _call_back)
	{
		for (auto *entity : entities)
		{
			_call_back(entity);
		}
	}

	Entity *World::GetByTag(const string &_tag) const
	{
		for (auto *entity : entities)
		{
			if (entity->Is(_tag))
			{
				return entity;
			}
		}
	}

}
