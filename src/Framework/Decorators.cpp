#include "stdafx.h"
#include "BehaviorTree.h"

#include "Decorators.h"

namespace Engine
{
	namespace BehaviorTree
	{
		
		Decorator::Decorator(const string &_name)
			:	Parrent(_name)
		{

		}
		
		void Decorator::AddChild(Node *_child)
		{
			if (child)
			{
				cout << "Decorator '" << name << "' can't have more than one child" << endl;
			}

			child.reset(_child);
		}

		Inverter::Inverter(const string &_name)
			: Decorator(_name)
		{

		}

		Status Inverter::Update()
		{
			auto status = child->Update();

			return status == Status::Success ? Status::Failure : Status::Success;
		}

// 		Repeater::Repeater(const string &_name)
// 			:	Decorator(_name)
// 		{
// 
// 		}
// 
// 		Status Repeater::Update()
// 		{
// 
// 		}
// 
// 		Succeeder::Succeeder(const string &_name)
// 			:	Decorator(_name)
// 		{
// 
// 		}
// 
// 		Status Succeeder::Update()
// 		{
// 
// 		}
// 
// 		Failer::Failer(const string &_name)
// 			:	Decorator(_name)
// 		{
// 
// 		}
// 
// 		Status Failer::Update()
// 		{
// 
// 		}

	}
}