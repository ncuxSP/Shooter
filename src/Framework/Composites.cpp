#include "stdafx.h"
#include "BehaviorTree.h"

#include "Composites.h"

namespace Engine
{
	namespace BehaviorTree
	{

		Composite::Composite(const string &_name)
			: Parent(_name)
		{

		}

		void Composite::AddChild(Node *_child)
		{
			children.emplace_back(_child);
		}

		Parallel::Parallel(const string &_name)
			: Composite(_name)
		{

		}

		Status Parallel::Update()
		{
			cout << name << endl;

			bool success = true;
			bool failure = true;

			for (auto &child : children)
			{
				auto status = child->Update();

				success &= status == Status::Success;
				failure &= status == Status::Failure;
			}

			if (success)
			{
				return Status::Success;
			}

			if (failure)
			{
				return Status::Failure;
			}

			return Status::Running;
		}

		Selector::Selector(const string &_name)
			: Composite(_name)
			, last(0)
		{

		}

		Status Selector::Update()
		{
			cout << name << endl;

			for (auto &child : children)
			{
				auto status = child->Update();

				if (status != Status::Failure)
				{
					return status;
				}
			}
			return Status::Failure;
		}

		Sequence::Sequence(const string &_name)
			: Composite(_name)
			, last(0)
		{

		}

		Status Sequence::Update()
		{
			cout << name << endl;

			for (auto &child : children)
			{
				auto status = child->Update();

				if (status != Status::Success)
				{
					return status;
				}
			}
			// 
			// 			for (auto i = last; i < children.size(); ++i)
			// 			{
			// 				auto &child = children[i];
			// 
			// 				auto status = child->Update();
			// 
			// 				if (status != Status::Success)
			// 				{
			// 					if (status == Status::Running)
			// 					{
			// 						last = i;
			// 					}
			// 					else
			// 					{
			// 						last = 0;
			// 					}
			// 					return status;
			// 				}
			// 			}
			// 			last = 0;

			return Status::Success;
		}

	}
}