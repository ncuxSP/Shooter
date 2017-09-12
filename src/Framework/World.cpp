#include "stdafx.h"

namespace Engine
{
	World::World()
		:	last_id(0)
	{
	
	}
	
	World::~World()
	{
		for (auto *entity : entities)
		{
			Emit<Events::OnEntityRemoved>({ entity });
			delete entity;
		}
		for (auto *system : systems)
		{
			system->UnConfigure(this);
			delete system;
		}
	}
	
	Entity *World::Create()
	{
		++last_id;
		auto entity = new Entity(this, last_id);
		entities.push_back(entity);

		Emit<Events::OnEntityCreated>({ entity });

		return entity;
	}
	
	void World::Destroy(Entity *_entity)
	{
		entities.erase(remove(entities.begin(), entities.end(), _entity), entities.end());
		Emit<Events::OnEntityRemoved>({ _entity });

		delete _entity;
		_entity = nullptr;
	}
	
	void World::Update(float _dt)
	{
		for (auto *system : systems)
		{
			system->Update(this, _dt);
		}
	}
	
	void World::Reset()
	{
		for (auto *entity : entities)
		{
			Emit<Events::OnEntityRemoved>({ entity });
			delete entity;
		}
		entities.clear();
		last_id = 0;
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
	
	void World::All(function<void(Entity *)> _call_back)
	{
		for (auto *entity : entities)
		{
			_call_back(entity);
		}
	}
}
