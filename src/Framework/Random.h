#pragma once

namespace Engine
{
	class Random
	{
	public:
		Random()
			: mt(_Random_device())
		{}

		template <class ValueType>
		ValueType Get(ValueType _min, ValueType _max)
		{
			if constexpr (is_integral_v<ValueType>) {
				uniform_int_distribution<ValueType> idis(_min, _max);
				return idis(mt);
			}
			else if constexpr (is_floating_point_v<ValueType>) {
				uniform_real_distribution<ValueType> rdis(_min, _max);
				return rdis(mt);
			}
		}

		template <class ValueType, class = enable_if_t<is_arithmetic_v<ValueType>>>
		ValueType GetFromZero(ValueType _max)
		{
			return Get<ValueType>(static_cast<ValueType>(0), _max);
		}

		template <class ValueType, class = enable_if_t<is_floating_point_v<ValueType>>>
		ValueType GetUnitRange()
		{
			return Get(static_cast<ValueType>(0), _max);
		}

	private:
		mt19937 mt;
	};
}