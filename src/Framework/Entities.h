#pragma once

namespace Engine
{
	class World;

	namespace Internal
	{
		using TypeIndex = type_index;
		template <class T>
		TypeIndex GetTypeIndex()
		{
			return type_index(typeid(T));
		}

		struct BaseComponent
		{
			virtual ~BaseComponent() = 0 {}
		};

		template <class T>
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

	template <class T>
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

		T *Get()
		{
			return component;
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
		Entity(World *_world, uint32_t _id, const string &_tag)
			:	world(_world)
			,	id(_id)
			,	tag(_tag)
		{

		}

		~Entity()
		{
			RemoveAll();
		}

		template <class T, typename... Args>
		ComponentPtr<T> Assign(Args&&... _args);

		template <class T>
		ComponentPtr<T> Get();

		template <class T>
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

		void RemoveAll();
		
		template <class T>
		bool Has() const
		{
			auto index = Internal::GetTypeIndex<T>();
			return components.find(index) != components.end();
		}

		template <class T1, class T2, typename... Types>
		bool Has() const
		{
			return Has<T1>() && Has<T2, Types...>();
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

		template <class T>
		struct OnComponentAssigned
		{
			Entity *entity;
			ComponentPtr<T> component;
		};
	}

	template <class T>
	class EventSubscriber : public Internal::BaseSubscriber
	{
	public:
		virtual ~EventSubscriber() override {}

		virtual void Receive(World *_world, const T &_event) = 0;
	};


	inline void Entity::RemoveAll()
	{
		for (auto pair : components)
		{
			delete pair.second;
		}
		components.clear();
	}

	template <class T, typename... Args>
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

	template <class T>
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

	inline uint32_t Entity::GetId() const
	{
		return id;
	}

	inline const string & Entity::GetTag() const
	{
		return tag;
	}

	inline bool Entity::Is(const string &_tag) const
	{
		return tag == _tag;
	}

}
