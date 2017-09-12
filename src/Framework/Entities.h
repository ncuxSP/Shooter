#pragma once

namespace Engine
{
	class World;

	namespace Internal
	{
		typedef type_index TypeIndex;
		template <typename T>
		TypeIndex GetTypeIndex()
		{
			return type_index(typeid(T));
		}

		struct BaseComponent
		{
			virtual ~BaseComponent() = 0 {}
		};

		template <typename T>
		struct Component : BaseComponent
		{
			Component() {}
			Component(const T &_data) : data(_data) {}
			virtual ~Component() override {}
			T data;
		};

		class BaseSubscriber
		{
		public:
			virtual ~BaseSubscriber() = 0 {}
		};
	}

	template <typename T>
	class ComponentPtr
	{
	public:
		ComponentPtr()
			:	component(nullptr)
		{

		}

		ComponentPtr(T *component)
			:	component(component)
		{

		}

		T *operator->() const
		{
			return component;
		}

		operator bool() const
		{
			return IsValid();
		}

		T &Get()
		{
			return *component;
		}

		bool IsValid() const
		{
			return component != nullptr;
		}

	private:
		T *component;
	};
	
	class EntitySystem
	{
	public:
		virtual ~EntitySystem() {}

		virtual void Configure(World* _world) {}

		virtual void UnConfigure(World* _world) {}

		virtual void Update(World* _world, float _dt) {}
	};

	class Entity
	{
	public:
		Entity(World *_world, uint32_t _id)
			: world(_world)
			, id(_id)
		{

		}

		~Entity()
		{
			RemoveAll();
		}

		template <typename T, typename... Args>
		ComponentPtr<T> Assign(Args&&... _args);

		template <typename T>
		ComponentPtr<T> Get();

		template <typename T>
		bool Remove()
		{
			auto found = components.find(Internal::GetTypeIndex<T>());
			if (found != components.end())
			{
				delete found->second;
				components.erase(found);
				return true;
			}
			return false;
		}

		inline void RemoveAll()
		{
			for (auto pair : components)
			{
				delete pair.second;
			}
			components.clear();
		}
		
		template <typename T>
		bool Has() const
		{
			auto index = Internal::GetTypeIndex<T>();
			return components.find(index) != components.end();
		}

		template <typename T1, typename T2, typename... Types>
		bool Has() const
		{
			return Has<T1>() && Has<T2, Types...>();
		}

		uint32_t GetId() const
		{
			return id;
		}

	private:
		unordered_map<Internal::TypeIndex, Internal::BaseComponent *> components;
		World *world;
		uint32_t id;
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

		template <typename T>
		struct OnComponentAssigned
		{
			Entity *entity;
			ComponentPtr<T> component;
		};
	}

	template <typename T>
	class EventSubscriber : public Internal::BaseSubscriber
	{
	public:
		virtual ~EventSubscriber() override {}

		virtual void Receive(World *_world, const T &_event) = 0;
	};

	template <typename T, typename... Args>
	ComponentPtr<T> Entity::Assign(Args&&... _args)
	{
		auto found = components.find(Internal::GetTypeIndex<T>());
		if (found != components.end())
		{
			auto component = reinterpret_cast<Internal::Component<T> *>(found->second);
			component->data = T(_args...);

			auto ptr = ComponentPtr<T>(&component->data);
			world->Emit<Events::OnComponentAssigned<T>>({ this, ptr });
			return ptr;
		}
		else
		{
			auto component = new Internal::Component<T>(T(_args...));
			components.insert({ Internal::GetTypeIndex<T>(), component });

			auto ptr = ComponentPtr<T>(&component->data);
			world->Emit<Events::OnComponentAssigned<T>>({ this, ptr });
			return ptr;
		}
	}

	template <typename T>
	ComponentPtr<T> Entity::Get()
	{
		auto found = components.find(Internal::GetTypeIndex<T>());
		if (found != components.end())
		{
			auto component = reinterpret_cast<Internal::Component<T> *>(found->second);
			return ComponentPtr<T>(&component->data);
		}
		else
		{
			return ComponentPtr<T>();
		}
	}

}
