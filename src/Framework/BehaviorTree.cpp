#include "stdafx.h"
#include "BehaviorTree.h"
#include "Composites.h"
#include "Decorators.h"

namespace Engine
{
	namespace BehaviorTree
	{

		Node::Node(const string &_name)
			:	name(_name)
		{

		}

		Node::~Node()
		{

		}

		Leaf::Leaf(const string &_name, function<Status(void)> _action)
			:	Node(_name)
			,	action(_action)
		{

		}

		Status Leaf::Update()
		{
			return action();
		}

		Parrent::Parrent(const string &_name)
			: Node(_name)
		{

		}

		void Parrent::AddChild(Node *_child)
		{

		}

		Builder::Builder()
		{

		}

		Builder &Builder::Action(const string &_name, function<Status(void)> _action)
		{
			if (parrents.empty())
			{
				cout << "Action can be Leaf only" << endl;
			}

			auto leaf = new Leaf(_name, _action);

			parrents.top()->AddChild(leaf);

			return *this;
		}

		Builder &Builder::Condition(const string &_name, function<bool(void)> _condition)
		{
			return Action(_name, [_condition]() -> Status
			{
				return _condition() ? Status::Success : Status::Failure;
			});
		}

		Builder &Builder::Inverter(const string &_name)
		{
			Add(new BehaviorTree::Inverter(_name));
			return *this;
		}
// 
// 		Builder &Builder::Failer(const string &_name)
// 		{
// 			// TODO: insert return statement here
// 		}
// 
// 		Builder &Builder::Repeater(const string &_name)
// 		{
// 			// TODO: insert return statement here
// 		}
// 
// 		Builder &Builder::Succeeder(const string &_name)
// 		{
// 			// TODO: insert return statement here
// 		}

		Builder &Builder::Selector(const string &_name)
		{
			Add(new BehaviorTree::Selector(_name));
			return *this;
		}

		Builder &Builder::Parallel(const string &_name)
		{
			Add(new BehaviorTree::Parallel(_name));
			return *this;
		}

		Builder &Builder::Sequence(const string &_name)
		{
			Add(new BehaviorTree::Sequence(_name));
			return *this;
		}

		Builder &Builder::End()
		{
			current = parrents.top();
			parrents.pop();
			return *this;
		}

		Node::Ptr Builder::Build()
		{
			return Node::Ptr(current);
		}

		void Builder::Add(Parrent *_node)
		{
			if (parrents.size() > 0)
			{
				parrents.top()->AddChild(_node);
			}
			parrents.push(_node);
		}

	}
}