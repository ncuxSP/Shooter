#pragma once

namespace Engine
{
	namespace BehaviorTree
	{
		enum class Status : uint8_t
		{
			Running,
			Success,
			Failure
		};

		class Node
		{
		public:
			Node(const string &_name);
			virtual ~Node();

			virtual Status Update() = 0;

		public:
			using Ptr = shared_ptr<Node>;

		protected:
			string name;
		};

		class Leaf : public Node
		{
		public:
			Leaf(const string &_name, function<Status(void)> _action);

			virtual Status Update() override;

		private:
			function<Status(void)> action;
		};

		class Parrent : public Node
		{
		public:
			Parrent(const string &_name);

			virtual void AddChild(Node *_child) = 0;
		};

		class Builder
		{
		public:
			Builder();

			//Leafs
			Builder &Action(const string &_name, function<Status(void)> _action);
			Builder &Condition(const string &_name, function<bool(void)> _condition);
			//Decorators
			Builder &Inverter(const string &_name);
// 			Builder &Repeater(const string &_name);
// 			Builder &Succeeder(const string &_name);
// 			Builder &Failer(const string &_name);
			//Composites
			Builder &Parallel(const string &_name);
			Builder &Selector(const string &_name);
			Builder &Sequence(const string &_name);
			Builder &End();
			//Get tree
			Node::Ptr Build();

		private:
			void Add(Parrent *_node);

		private:
			Node *current;
			stack<Parrent *> parrents;
		};
	}
}