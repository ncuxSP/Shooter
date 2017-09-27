#pragma once

namespace Engine
{
	class World
	{
	public:
		World();
		~World();

		Entity *Create(const string &_tag = "");

		void Destroy(Entity *_entity);

		void Update(float _dt);

		void Reset();

		void RegisterSystem(EntitySystem *_system);

		void UnRegisterSystem(EntitySystem *_system);
		
		template <class T>
		void Subscribe(EventSubscriber<T> *_subscriber);

		template <class T>
		void UnSubscribe(EventSubscriber<T> *_subscriber);

		void UnSubscribeAll(Internal::BaseSubscriber *_subscriber);

		template <class T>
		void Emit(const T &_event);

		template <typename... Types>
		void Each(typename common_type_t<function<void(Entity *, ComponentPtr<Types>...)>> _call_back);

		void EachTag(const string &_tag, function<void(Entity *)> _call_back);

		void All(function<void(Entity *)> _call_back);

		Entity *GetByTag(const string &_tag) const;

	private:
		vector<Entity *> entities;
		vector<Entity *> new_entities;
		vector<Entity *> removed_entities;
		vector<EntitySystem *> systems;
		unordered_map<Internal::TypeIndex, vector<Internal::BaseSubscriber *>> subscribers;
		uint32_t last_id;
		bool start_reset;
	};

	template <class T>
	void World::Subscribe(EventSubscriber<T> *_subscriber)
	{
		auto index = Internal::GetTypeIndex<T>();
		auto found = subscribers.find(index);
		if (found == subscribers.end())
		{
			vector<Internal::BaseSubscriber *> subs;
			subs.push_back(_subscriber);
			subscribers.insert({ index, subs });
		}
		else
		{
			found->second.push_back(_subscriber);
		}
	}

	template <class T>
	void World::UnSubscribe(EventSubscriber<T> *_subscriber)
	{
		auto index = Internal::GetTypeIndex<T>();
		auto found = subscribers.find(index);
		if (found != subscribers.end())
		{
			found->second.erase(remove(found->second.begin(), found->second.end(), _subscriber), found->second.end());
			if (found->second.size() == 0)
			{
				subscribers.erase(found);
			}
		}
	}

	template <class T>
	void World::Emit(const T &_event)
	{
		auto index = Internal::GetTypeIndex<T>();
		auto found = subscribers.find(index);
		if (found != subscribers.end())
		{
			for (auto *base : found->second)
			{
				auto *sub = reinterpret_cast<EventSubscriber<T> *>(base);
				sub->Receive(this, _event);
			}
		}
	}

	template <typename... Types>
	void World::Each(typename common_type_t<function<void(Entity *, ComponentPtr<Types>...)>> _call_back)
	{
		for (auto *entity : entities)
		{
			if (entity->template Has<Types...>())
			{
				_call_back(entity, entity->template Get<Types>()...);
			}
		}
	}
}
