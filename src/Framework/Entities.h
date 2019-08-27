#pragma once

namespace Engine
{
	namespace Framework
	{
		class World;

		namespace Internal
		{
			using TypeIndex = type_index;

			template <class AnyType>
			TypeIndex GetTypeIndex()
			{
				return TypeIndex(typeid(AnyType));
			}

			class BaseComponent
			{
			public:
				virtual ~BaseComponent() = 0 {}
			};

			template <class AnyStruct>
			class Component : public BaseComponent
			{
			public:
				Component() {}
				Component(const AnyStruct &_data) : data(_data) {}
				virtual ~Component() override {}
				AnyStruct data;
			};

			class BaseSubscriber
			{
			public:
				virtual ~BaseSubscriber() = 0 {}
			};
		}

		template <class AnyStruct>
		class ComponentPtr
		{
		public:
			ComponentPtr()
				: component(nullptr)
			{

			}

			ComponentPtr(AnyStruct *component)
				: component(component)
			{

			}

			AnyStruct *operator->() const
			{
				return component;
			}

			operator bool() const
			{
				return IsValid();
			}

			AnyStruct *Get()
			{
				return component;
			}

			bool IsValid() const
			{
				return component != nullptr;
			}

		private:
			AnyStruct *component;
		};

		class Entity;

		class EntitySystem
		{
		public:
			virtual ~EntitySystem() {}

			virtual void Configure(World *_world) {}

			virtual void UnConfigure(World *_world) {}

			virtual void Update(World *_world, float _dt) {}
		};

		namespace Events
		{
			struct OnEntityCreated
			{
				Entity *entity;
			};

			struct OnEntityRemoved
			{
				Entity *entity;
			};

			template <class AnyStruct>
			struct OnComponentAssigned
			{
				Entity *entity;
				ComponentPtr<AnyStruct> component;
			};
		}

		template <class EventType>
		class EventSubscriber : public Internal::BaseSubscriber
		{
		public:
			virtual ~EventSubscriber() override {}

			virtual void Receive(World *_world, const EventType &_event) = 0;
		};

		class Entity
		{
		public:
			Entity(World *_world, uint32_t _id, const string &_tag)
				: world(_world)
				, id(_id)
				, tag(_tag)
			{

			}

			~Entity()
			{
				RemoveAll();
			}

			template <class AnyStruct, typename... ConstructorArgs>
			ComponentPtr<AnyStruct> Assign(ConstructorArgs &&... _args);

			template <class AnyStruct>
			ComponentPtr<AnyStruct> Get();

			template <class AnyStruct>
			bool Remove();

			void RemoveAll();

			template <class AnyStruct>
			bool Has() const
			{
				auto index = Internal::GetTypeIndex<AnyStruct>();
				return components.find(index) != components.end();
			}

			template <class C1, class C2, typename... OtherComponents>
			bool Has() const
			{
				return Has<C1>() && Has<C2, OtherComponents...>();
			}

			uint32_t GetId() const;

			const string &GetTag() const;

			bool Is(const string &_tag) const;

		private:
			unordered_map<Internal::TypeIndex, Internal::BaseComponent *> components;
			World *world;
			uint32_t id;
			string tag;
		};


		inline void Entity::RemoveAll()
		{
			for (auto pair : components)
			{
				delete pair.second;
			}
			components.clear();
		}

		template <class AnyStruct, typename... ConstructorArgs>
		ComponentPtr<AnyStruct> Entity::Assign(ConstructorArgs &&... _args)
		{
			Internal::Component<AnyStruct> *component;
			auto found = components.find(Internal::GetTypeIndex<AnyStruct>());
			if (found != components.end())
			{
				component = reinterpret_cast<Internal::Component<AnyStruct> *>(found->second);
				component->data = AnyStruct(_args...);
			}
			else
			{
				component = new Internal::Component<AnyStruct>(AnyStruct(_args...));
				components.insert({ Internal::GetTypeIndex<AnyStruct>(), component });
			}

			auto ptr = ComponentPtr<AnyStruct>(&component->data);
			world->Emit<Events::OnComponentAssigned<AnyStruct>>({ this, ptr });

			return ptr;
		}

		template <class AnyStruct>
		ComponentPtr<AnyStruct> Entity::Get()
		{
			auto found = components.find(Internal::GetTypeIndex<AnyStruct>());
			if (found != components.end())
			{
				auto component = reinterpret_cast<Internal::Component<AnyStruct> *>(found->second);
				return ComponentPtr<AnyStruct>(&component->data);
			}
			else
			{
				return ComponentPtr<AnyStruct>();
			}
		}

		template<class AnyStruct>
		bool Entity::Remove()
		{
			auto found = components.find(Internal::GetTypeIndex<AnyStruct>());
			if (found != components.end())
			{
				delete found->second;
				components.erase(found);

				return true;
			}

			return false;
		}

		inline uint32_t Entity::GetId() const
		{
			return id;
		}

		inline const string &Entity::GetTag() const
		{
			return tag;
		}

		inline bool Entity::Is(const string &_tag) const
		{
			return tag == _tag;
		}
	}
}