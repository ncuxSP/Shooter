#pragma once

namespace Engine
{
	class Random
	{
	public:
		Random()
			:	mt(_Random_device())
		{}

		template <class T, typename enable_if_t<is_integral<T>::value>* = 0>
		T Get(T _min, T _max)
		{
			uniform_int_distribution<T> idis(_min, _max);
			return idis(mt);
		}

		template <class T, typename enable_if_t<is_floating_point<T>::value>* = 0>
		T Get(T _min, T _max)
		{
			uniform_real_distribution<T> rdis(_min, _max);
			return rdis(mt);
		}

		template <class T>
		T GetFromZero(T _max)
		{
			return Get<T>(static_cast<T>(0), _max);
		}

		template <class T, typename enable_if_t<is_floating_point<T>::value>* = 0>
		T GetUnitRange()
		{
			return Get(static_cast<T>(0), _max);
		}
		
	private:
		mt19937 mt;
	};
}