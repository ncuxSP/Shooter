#pragma once

namespace Engine
{
	namespace BehaviorTree
	{
		class Composite : public Parrent
		{
		public:
			Composite(const string &_name);

			virtual void AddChild(Node *_child) override;

		protected:
			vector<unique_ptr<Node>> children;
		};

		class Parallel : public Composite
		{
		public:
			Parallel(const string &_name);

			virtual Status Update() override;
		};

		class Selector : public Composite
		{
		public:
			Selector(const string &_name);

			virtual Status Update() override;
		};

		class Sequence : public Composite
		{
		public:
			Sequence(const string &_name);

			virtual Status Update() override;
		};
	}
}