#pragma once

namespace Engine
{
	class Random
	{
	public:
		Random()
			: mt(_Random_device())
		{}

		template <class ValueType, typename enable_if_t<is_integral<ValueType>::value> * = 0>
		ValueType Get(ValueType _min, ValueType _max)
		{
			uniform_int_distribution<ValueType> idis(_min, _max);
			return idis(mt);
		}

		template <class ValueType, typename enable_if_t<is_floating_point<ValueType>::value> * = 0>
		ValueType Get(ValueType _min, ValueType _max)
		{
			uniform_real_distribution<ValueType> rdis(_min, _max);
			return rdis(mt);
		}

		template <class ValueType>
		ValueType GetFromZero(ValueType _max)
		{
			return Get<ValueType>(static_cast<ValueType>(0), _max);
		}

		template <class ValueType, typename enable_if_t<is_floating_point<ValueType>::value> * = 0>
		ValueType GetUnitRange()
		{
			return Get(static_cast<ValueType>(0), _max);
		}

	private:
		mt19937 mt;
	};
}