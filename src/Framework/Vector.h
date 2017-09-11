#pragma once

namespace Engine
{
	template<typename Type> 
	inline bool math_vector_equal(Type _a, Type _b)
	{
		return (_a == _b);
	}

	inline float math_vector_equal(float _a, float _b)
	{
		return (fabs(_a - _b) < std::numeric_limits<float>::epsilon());
	}

	template<typename Type, uint32_t Size> 
	class math_vector_base
	{
	public:
		Type v[Size];
	};

	template<typename Type>
	class math_vector_base <Type, 2>
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
		inline math_vector_base()
		{
		}

		inline math_vector_base(Type _x, Type _y)
			: x(_x), y(_y)
		{
		}
	};

	template<typename Type> 
	class math_vector_base <Type, 3>
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
		inline math_vector_base()
		{
		}

		inline math_vector_base(Type _x, Type _y, Type _z)
			: x(_x), y(_y), z(_z)
		{
		}
	};

	template<typename Type>
	class math_vector_base <Type, 4>
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
		inline math_vector_base()
		{
		}

		inline math_vector_base(Type _x, Type _y, Type _z, Type _w)
			: x(_x), y(_y), z(_z), w(_w)
		{
		}

		inline math_vector_base(Type _rgb, Type _a)
			: x(_rgb), y(_rgb), z(_rgb), w(_a)
		{
		}
	};

	template<typename Type, uint32_t Size> 
	class math_vector : public math_vector_base<Type, Size>
	{
	public:
		inline math_vector()
		{
		}

		inline math_vector(const math_vector &_vector)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				math_vector_base<Type, Size>::v[i] = static_cast<Type>(_vector.v[i]);
			}
		}

		template<typename _Type, uint32_t _Size>
		inline math_vector(const math_vector<_Type, _Size> &_vector)
		{
			uint32_t MIN_SIZE = Size < _Size ? Size : _Size;
			
			for (uint32_t i = 0; i < MIN_SIZE; ++i)
			{
				math_vector_base<Type, Size>::v[i] = static_cast<Type>(_vector.v[i]);
			}

			for (uint32_t i = _Size; i < Size; ++i)
			{
				math_vector_base<Type, Size>::v[i] = static_cast<Type>(0);
			}
		}

		inline math_vector(const Type *_v)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				math_vector_base<Type, Size>::v[i] = _v[i];
			}
		}

		inline math_vector(Type _value)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				math_vector_base<Type, Size>::v[i] = _value;
			}
		}

		inline math_vector(Type _x, Type _y)
			: math_vector_base<Type, Size>(_x, _y)
		{
		}

		inline math_vector(Type _x, Type _y, Type _z)
			: math_vector_base<Type, Size>(_x, _y, _z)
		{
		}

		inline math_vector(Type _x, Type _y, Type _z, Type _w)
			: math_vector_base<Type, Size>(_x, _y, _z, _w)
		{
		}

		inline Type length_square() const
		{
			Type length = static_cast<Type>(0);

			for (uint32_t i = 0; i < Size; ++i)
			{
				length += math_vector_base<Type, Size>::v[i] * math_vector_base<Type, Size>::v[i];
			}

			return length;
		}

		inline float length() const
		{
			return sqrtf(static_cast<float>(length_square()));
		}

		inline void normalize()
		{
			set_length(static_cast<Type>(1));
		}

		inline math_vector get_normalized() const
		{
			math_vector normalized_vector = *this;
			normalized_vector.normalize();

			return normalized_vector;
		}

		inline void set_length(float _length)
		{
			float magnitude = length() / _length;

			if (fabs(magnitude) < M_EPSILON)
			{
				return;
			}

			*this /= magnitude;
		}

		inline void copy(Type *_v)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				_v[i] = math_vector_base<Type, Size>::v[i];
			}
		}

		inline math_vector operator = (const math_vector &_vector)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				math_vector_base<Type, Size>::v[i] = _vector.v[i];
			}

			return *this;
		}

		template<typename _Type, uint32_t _Size>
		inline math_vector<Type, Size> operator = (const math_vector<_Type, _Size> &_vector)
		{
			uint32_t MIN_SIZE = Size < _Size ? Size : _Size;
			
			for (uint32_t i = 0; i < MIN_SIZE; ++i)
			{
				math_vector_base<Type, Size>::v[i] = static_cast<Type>(_vector.v[i]);
			}

			for (uint32_t i = _Size; i < Size; ++i)
			{
				math_vector_base<Type, Size>::v[i] = static_cast<Type>(0);
			}

			return *this;
		}

		inline math_vector operator + (const math_vector &_vector) const
		{
			math_vector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = math_vector_base<Type, Size>::v[i] + _vector.v[i];
			}

			return new_vector;
		}

		template<typename _Type>
		inline math_vector<Type, Size> operator + (const math_vector<_Type, Size> &_vector) const
		{
			math_vector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = math_vector_base<Type, Size>::v[i] + static_cast<Type>(_vector.v[i]);
			}

			return new_vector;
		}

		inline math_vector operator - (const math_vector &_vector) const
		{
			math_vector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = math_vector_base<Type, Size>::v[i] - _vector.v[i];
			}

			return new_vector;
		}

		template<typename _Type>
		inline math_vector<Type, Size> operator - (const math_vector<_Type, Size> &_vector) const
		{
			math_vector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = math_vector_base<Type, Size>::v[i] - static_cast<Type>(_vector.v[i]);
			}

			return new_vector;
		}

		inline math_vector operator * (const math_vector &_vector) const
		{
			math_vector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = math_vector_base<Type, Size>::v[i]  *_vector.v[i];
			}

			return new_vector;
		}

		template<typename _Type>
		inline math_vector<Type, Size> operator * (const math_vector<_Type, Size> &_vector) const
		{
			math_vector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = static_cast<Type>(math_vector_base<Type, Size>::v[i]  *_vector.v[i]);
			}

			return new_vector;
		}

		inline math_vector operator / (const math_vector &_vector) const
		{
			math_vector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = math_vector_base<Type, Size>::v[i] / _vector.v[i];
			}

			return new_vector;
		}

		template<typename _Type>
		inline math_vector<Type, Size> operator / (const math_vector<_Type, Size> &_vector) const
		{
			math_vector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = static_cast<Type>(math_vector_base<Type, Size>::v[i] / _vector.v[i]);
			}

			return new_vector;
		}

		template<typename _Type>
		inline math_vector<Type, Size> operator * (_Type _scale) const
		{
			math_vector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = static_cast<Type>(math_vector_base<Type, Size>::v[i]  *_scale);
			}

			return new_vector;
		}

		template<typename _Type>
		inline math_vector operator / (_Type _scale) const
		{
			math_vector<Type, Size> new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = static_cast<Type>(math_vector_base<Type, Size>::v[i] / _scale);
			}

			return new_vector;
		}

		inline math_vector operator - (void) const
		{
			math_vector new_vector;

			for (uint32_t i = 0; i < Size; ++i)
			{
				new_vector.v[i] = -math_vector_base<Type, Size>::v[i];
			}

			return new_vector;
		}

		inline math_vector & operator += (const math_vector &_vector)
		{
			*this = *this + _vector;

			return *this;
		}

		template<typename _Type>
		inline math_vector<Type, Size> & operator += (const math_vector<_Type, Size> &_vector)
		{
			*this = *this + _vector;

			return *this;
		}

		inline math_vector & operator -= (const math_vector &_vector)
		{
			*this = *this - _vector;

			return *this;
		}

		template<typename _Type>
		inline math_vector<Type, Size> & operator -= (const math_vector<_Type, Size> &_vector)
		{
			*this = *this - _vector;

			return *this;
		}

		inline math_vector & operator *= (const math_vector &_vector)
		{
			*this = *this  *_vector;

			return *this;
		}

		template<typename _Type>
		inline math_vector<Type, Size> & operator *= (const math_vector<_Type, Size> &_vector)
		{
			*this = *this  *_vector;

			return *this;
		}

		inline math_vector & operator /= (const math_vector &_vector)
		{
			*this = *this / _vector;

			return *this;
		}

		template<typename _Type>
		inline math_vector<Type, Size> & operator /= (const math_vector<_Type, Size> &_vector)
		{
			*this = *this / _vector;

			return *this;
		}

		template<typename _Type>
		inline math_vector & operator *= (_Type _scale)
		{
			*this = *this  *_scale;

			return *this;
		}

		template<typename _Type>
		inline math_vector & operator /= (_Type _scale)
		{
			*this = *this / _scale;

			return *this;
		}

		inline bool operator == (const math_vector &_vector) const
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				if (!math_vector_equal(math_vector_base<Type, Size>::v[i], _vector.v[i]))
				{
					return false;
				}
			}

			return true;
		}

		inline bool operator != (const math_vector &_vector) const
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				if (!math_vector_equal(math_vector_base<Type, Size>::v[i], _vector.v[i]))
				{
					return true;
				}
			}

			return false;
		}

		inline bool operator <= (const math_vector &_vector) const
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				if (math_vector_base<Type, Size>::v[i] > _vector.v[i])
				{
					return false;
				}
			}

			return true;
		}

		inline bool operator >= (const math_vector &_vector) const
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				if (math_vector_base<Type, Size>::v[i] < _vector.v[i])
				{
					return false;
				}
			}

			return true;
		}

		inline bool operator < (const math_vector &_vector) const
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				if (math_vector_base<Type, Size>::v[i] >= _vector.v[i])
				{
					return false;
				}
			}

			return true;
		}

		inline bool operator > (const math_vector &_vector) const
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				if (math_vector_base<Type, Size>::v[i] <= _vector.v[i])
				{
					return false;
				}
			}

			return true;
		}
	};

	template<typename Type, uint32_t Size>
	inline Type dot_product(const math_vector<Type, Size> &_a, const math_vector<Type, Size> &_b)
	{
		Type result = static_cast<Type>(0);

		for (uint32_t i = 0; i < Size; ++i)
		{
			result += _a.v[i]  *_b.v[i];
		}

		return result;
	}

	template<typename Type>
	inline math_vector<Type, 3> cross_product(const math_vector<Type, 3> &_a, const math_vector<Type, 3> &_b)
	{
		math_vector<Type, 3> result;

		result.x = _a.y  *_b.z - _a.z  *_b.y;
		result.y = _a.z  *_b.x - _a.x  *_b.z;
		result.z = _a.x  *_b.y - _a.y  *_b.x;

		return result;
	}

	template<typename Type, uint32_t Size>
	inline bool is_inside_segment(const math_vector<Type, Size> &_a, const math_vector<Type, Size> &_b,	const math_vector<Type, Size> &_point)
	{
		return (dot_product((_a - _point), (_b - _point)) <= 0);
	}

	typedef math_vector<float, 2>		vec2f;
	typedef math_vector<float, 3>		vec3f;

	typedef math_vector<int32_t, 2>		vec2i;
	typedef math_vector<int32_t, 3>		vec3i;

	typedef math_vector<uint32_t, 2>	vec2ui;
	typedef math_vector<uint32_t, 3>	vec3ui;
	typedef math_vector<uint32_t, 4>	vec4ui;

	typedef math_vector<uint8_t, 4>		vec4ui8;

	using Size = vec2ui;
	using Point = vec2i;
	using Vector = vec2f;
	using Color = vec4ui8;
}
