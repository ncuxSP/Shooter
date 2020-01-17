#pragma once

namespace Engine
{
	namespace Framework
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

			template <class EventType>
			void Subscribe(EventSubscriber<EventType> *_subscriber);

			template <class EventType>
			void UnSubscribe(EventSubscriber<EventType> *_subscriber);

			void UnSubscribeAll(Internal::BaseSubscriber *_subscriber);

			template <class EventType>
			void Emit(const EventType &_event);

			template <typename... ArgumentTypes>
			void Each(common_type_t<function<void(Entity *, ComponentPtr<ArgumentTypes>...)>> _call_back);

			void EachTag(const string &_tag, function<void(Entity *)> _call_back);

			void All(function<void(Entity *)> _call_back);

			Entity *GetByTag(const string &_tag) const;

		private:
			vector<Entity *> entities;
			vector<Entity *> created_entities;
			vector<Entity *> removed_entities;
			vector<EntitySystem *> systems;
			unordered_map<Internal::TypeIndex, vector<Internal::BaseSubscriber *>> subscribers;
			uint32_t last_id;
			bool start_reset;
		};

		template <class EventType>
		void World::Subscribe(EventSubscriber<EventType> *_subscriber)
		{
			auto index = Internal::GetTypeIndex<EventType>();
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

		template <class EventType>
		void World::UnSubscribe(EventSubscriber<EventType> *_subscriber)
		{
			auto index = Internal::GetTypeIndex<EventType>();
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

		template <class EventType>
		void World::Emit(const EventType &_event)
		{
			auto index = Internal::GetTypeIndex<EventType>();
			auto found = subscribers.find(index);
			if (found != subscribers.end())
			{
				for (auto *base : found->second)
				{
					auto *sub = reinterpret_cast<EventSubscriber<EventType> *>(base);
					sub->Receive(this, _event);
				}
			}
		}

		template <typename... ArgumentTypes>
		void World::Each(common_type_t<function<void(Entity *, ComponentPtr<ArgumentTypes>...)>> _call_back)
		{
			for (auto *entity : entities)
			{
				if (entity->Has<ArgumentTypes...>())
				{
					_call_back(entity, entity->Get<ArgumentTypes>()...);
				}
			}
		}
	}
}
