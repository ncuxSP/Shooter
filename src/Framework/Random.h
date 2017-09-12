#pragma once

namespace Engine
{
	class Random
	{
	public:
		Random(int min, int max)
			: mt(_Random_device())
			, idis(min, max)
		{}

		int Next()
		{
			return idis(mt);
		}
	private:
		mt19937 mt;
		uniform_int_distribution<int> idis;
	};
}