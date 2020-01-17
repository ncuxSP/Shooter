#pragma once

namespace Engine
{
	namespace BehaviorTree
	{
		class Decorator : public Parent
		{
		public:
			Decorator(const string &_name);

			void AddChild(Node *_child) override;

		protected:
			unique_ptr<Node> child;
		};

		class Inverter : public Decorator
		{
		public:
			Inverter(const string &_name);

			Status Update() override;
		};

		// 		class Repeater : public Decorator
		// 		{
		// 		public:
		// 			Repeater(const string &_name);
		// 
		// 			virtual Status Update() override;
		// 		};
		// 
		// 		class Succeeder : public Decorator
		// 		{
		// 		public:
		// 			Succeeder(const string &_name);
		// 
		// 			virtual Status Update() override;
		// 		};
		// 
		// 		class Failer : public Decorator
		// 		{
		// 		public:
		// 			Failer(const string &_name);
		// 
		// 			virtual Status Update() override;
		// 		};
	}
}

