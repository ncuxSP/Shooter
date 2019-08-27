#pragma once

namespace Engine
{
	template<class Type>
	inline bool MathVectorEqual(Type _a, Type _b)
	{
		return (_a == _b);
	}

	inline bool MathVectorEqual(float _a, float _b)
	{
		return (fabs(_a - _b) < numeric_limits<float>::epsilon());
	}

	template<class Type, uint32_t Size>
	class MathVectorBase
	{
	public:
		MathVectorBase()
		{}
	public:
		Type v[Size];
	};

	template<class Type, uint32_t Size>
	class MathVector : public MathVectorBase<Type, Size>
	{
	public:
		inline MathVector()
		{
		}

		inline MathVector(const MathVector &_vector)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				MathVectorBase<Type, Size>::v[i] = static_cast<Type>(_vector.v[i]);
			}
		}

		template<class _Type, uint32_t _Size>
		inline MathVector(const MathVector<_Type, _Size> &_vector)
		{
			const uint32_t min_size = Size < _Size ? Size : _Size;

			for (uint32_t i = 0; i < min_size; ++i)
			{
				MathVectorBase<Type, Size>::v[i] = static_cast<Type>(_vector.v[i]);
			}

			for (uint32_t i = _Size; i < Size; ++i)
			{
				MathVectorBase<Type, Size>::v[i] = static_cast<Type>(0);
			}
		}

		inline MathVector(const Type *_v)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				MathVectorBase<Type, Size>::v[i] = _v[i];
			}
		}

		inline MathVector(Type _value)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				MathVectorBase<Type, Size>::v[i] = _value;
			}
		}

		inline MathVector(Type _x, Type _y)
			: MathVectorBase<Type, Size>(_x, _y)
		{
		}

		inline MathVector(Type _x, Type _y, Type _z)
			: MathVectorBase<Type, Size>(_x, _y, _z)
		{
		}

		inline MathVector(Type _x, Type _y, Type _z, Type _w)
			: MathVectorBase<Type, Size>(_x, _y, _z, _w)
		{
		}

		inline Type LengthSquare() const
		{
			Type length = static_cast<Type>(0);

			for (uint32_t i = 0; i < Size; ++i)
			{
				length += MathVectorBase<Type, Size>::v[i] * MathVectorBase<Type, Size>::v[i];
			}

			return length;
		}

		inline float Length() const
		{
			return sqrtf(static_cast<float>(LengthSquare()));
		}

		inline void Normalize()
		{
			SetLength(static_cast<Type>(1));
		}

		inline MathVector GetNormalized() const
		{
			MathVector normalized_vector = *this;
			normalized_vector.Normalize();

			return normalized_vector;
		}

		inline void SetLength(float _length)
		{
			float magnitude = Length() / _length;

			if (fabs(magnitude) < std::numeric_limits<float>::epsilon())
			{
				return;
			}

			*this /= magnitude;
		}

		inline void Copy(Type *_v)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				_v[i] = MathVectorBase<Type, Size>::v[i];
			}
		}

		inline MathVector operator = (const MathVector &_vector)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				MathVectorBase<Type, Size>::v[i] = _vector.v[i];
			}

			return *this;
		}

		template<class _Type, uint32_t _Size>
		inline MathVector<Type, Size> operator = (const MathVector<_Type, _Size> &_vector)
		{
			const uint32_t min_size = Size < _Size ? Size : _Size;

			for (uint32_t i = 0; i < min_size; ++i)
			{
				MathVectorBase<Type, Size>::v[i] = static_cast<Type>(_vector.v[i]);
			}

			for (uint32_t i = _Size; i < Size; ++i)
			{
				MathVectorBase<Type, Size>::v[i] = static_cast<Type>(0);
			}

			return *this;
		}

		inline MathVector operator + (const MathVector &_vector) const
		{
			MathVector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = MathVectorBase<Type, Size>::v[i] + _vector.v[i];
			}

			return new_vector;
		}

		template<class _Type>
		inline MathVector<Type, Size> operator + (const MathVector<_Type, Size> &_vector) const
		{
			MathVector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = MathVectorBase<Type, Size>::v[i] + static_cast<Type>(_vector.v[i]);
			}

			return new_vector;
		}

		inline MathVector operator - (const MathVector &_vector) const
		{
			MathVector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = MathVectorBase<Type, Size>::v[i] - _vector.v[i];
			}

			return new_vector;
		}

		template<class _Type>
		inline MathVector<Type, Size> operator - (const MathVector<_Type, Size> &_vector) const
		{
			MathVector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = MathVectorBase<Type, Size>::v[i] - static_cast<Type>(_vector.v[i]);
			}

			return new_vector;
		}

		inline MathVector operator * (const MathVector &_vector) const
		{
			MathVector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = MathVectorBase<Type, Size>::v[i] * _vector.v[i];
			}

			return new_vector;
		}

		template<class _Type>
		inline MathVector<Type, Size> operator * (const MathVector<_Type, Size> &_vector) const
		{
			MathVector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = static_cast<Type>(MathVectorBase<Type, Size>::v[i] * _vector.v[i]);
			}

			return new_vector;
		}

		inline MathVector operator / (const MathVector &_vector) const
		{
			MathVector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = MathVectorBase<Type, Size>::v[i] / _vector.v[i];
			}

			return new_vector;
		}

		template<class _Type>
		inline MathVector<Type, Size> operator / (const MathVector<_Type, Size> &_vector) const
		{
			MathVector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = static_cast<Type>(MathVectorBase<Type, Size>::v[i] / _vector.v[i]);
			}

			return new_vector;
		}

		template<class _Type>
		inline MathVector<Type, Size> operator * (_Type _scale) const
		{
			MathVector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = static_cast<Type>(MathVectorBase<Type, Size>::v[i] * _scale);
			}

			return new_vector;
		}

		template<class _Type>
		inline MathVector operator / (_Type _scale) const
		{
			MathVector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = static_cast<Type>(MathVectorBase<Type, Size>::v[i] / _scale);
			}

			return new_vector;
		}

		inline MathVector operator - (void) const
		{
			MathVector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = -MathVectorBase<Type, Size>::v[i];
			}

			return new_vector;
		}

		inline MathVector &operator += (const MathVector &_vector)
		{
			*this = *this + _vector;

			return *this;
		}

		template<class _Type>
		inline MathVector<Type, Size> &operator += (const MathVector<_Type, Size> &_vector)
		{
			*this = *this + _vector;

			return *this;
		}

		inline MathVector &operator -= (const MathVector &_vector)
		{
			*this = *this - _vector;

			return *this;
		}

		template<class _Type>
		inline MathVector<Type, Size> &operator -= (const MathVector<_Type, Size> &_vector)
		{
			*this = *this - _vector;

			return *this;
		}

		inline MathVector &operator *= (const MathVector &_vector)
		{
			*this = *this * _vector;

			return *this;
		}

		template<class _Type>
		inline MathVector<Type, Size> &operator *= (const MathVector<_Type, Size> &_vector)
		{
			*this = *this * _vector;

			return *this;
		}

		inline MathVector &operator /= (const MathVector &_vector)
		{
			*this = *this / _vector;

			return *this;
		}

		template<class _Type>
		inline MathVector<Type, Size> &operator /= (const MathVector<_Type, Size> &_vector)
		{
			*this = *this / _vector;

			return *this;
		}

		template<class _Type>
		inline MathVector &operator *= (_Type _scale)
		{
			*this = *this * _scale;

			return *this;
		}

		template<class _Type>
		inline MathVector &operator /= (_Type _scale)
		{
			*this = *this / _scale;

			return *this;
		}

		inline bool operator == (const MathVector &_vector) const
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				if (!MathVectorEqual(MathVectorBase<Type, Size>::v[i], _vector.v[i]))
				{
					return false;
				}
			}

			return true;
		}

		inline bool operator != (const MathVector &_vector) const
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				if (!MathVectorEqual(MathVectorBase<Type, Size>::v[i], _vector.v[i]))
				{
					return true;
				}
			}

			return false;
		}
	};


	template<class Type>
	class MathVectorBase <Type, 2>
	{
	public:
		union
		{
			struct
			{
				Type x, y;
			};

			struct
			{
				Type w, h;
			};

			Type v[2];
		};

	public:
		inline MathVectorBase()
		{
		}

		inline MathVectorBase(Type _x, Type _y)
			: x(_x), y(_y)
		{
		}
	};

	template<class Type>
	class MathVectorBase <Type, 3>
	{
	public:
		union
		{
			struct
			{
				Type x, y, z;
			};

			struct
			{
				Type r, g, b;
			};

			Type v[3];
		};

	public:
		inline MathVectorBase()
		{
		}

		inline MathVectorBase(Type _x, Type _y, Type _z)
			: x(_x), y(_y), z(_z)
		{
		}
	};

	template<class Type>
	class MathVectorBase <Type, 4>
	{
	public:
		union
		{
			struct
			{
				Type x, y, z, w;
			};

			struct
			{
				Type r, g, b, a;
			};

			Type v[4];
		};

	public:
		inline MathVectorBase()
			: x(0), y(0), z(0), w(0)
		{
		}

		inline MathVectorBase(Type _x, Type _y, Type _z, Type _w)
			: x(_x), y(_y), z(_z), w(_w)
		{
		}

		inline MathVectorBase(Type _rgb, Type _a)
			: x(_rgb), y(_rgb), z(_rgb), w(_a)
		{
		}
	};

	namespace Math
	{
		template<class Type, uint32_t Size>
		inline Type DotProduct(const MathVector<Type, Size> &_a, const MathVector<Type, Size> &_b)
		{
			Type result = static_cast<Type>(0);

			for (uint32_t i = 0; i < Size; ++i)
			{
				result += _a.v[i] * _b.v[i];
			}

			return result;
		}

		template<class Type>
		inline MathVector<Type, 3> CrossProduct(const MathVector<Type, 3> &_a, const MathVector<Type, 3> &_b)
		{
			MathVector<Type, 3> result;

			result.x = _a.y * _b.z - _a.z * _b.y;
			result.y = _a.z * _b.x - _a.x * _b.z;
			result.z = _a.x * _b.y - _a.y * _b.x;

			return result;
		}

		template<class Type, uint32_t Size>
		inline bool IsInsideSegment(const MathVector<Type, Size> &_a, const MathVector<Type, Size> &_b, const MathVector<Type, Size> &_point)
		{
			return (DotProduct((_a - _point), (_b - _point)) <= 0);
		}
	}

	using Size = MathVector<int32_t, 2>;
	using Point = MathVector<int32_t, 2>;
	using Vector = MathVector<float, 2>;
	using Color = MathVector<uint8_t, 4>;
}
