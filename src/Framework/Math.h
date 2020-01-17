#pragma once

namespace Engine
{
	namespace Math
	{
		struct Rectangle
		{
			Point position;
			Size size;
			float angle;
		};

		template <class ValueType, class = enable_if_t<is_arithmetic_v<ValueType>>>
		ValueType Sign(ValueType _in)
		{
			auto zero = static_cast<ValueType>(0);
			auto plus = static_cast<ValueType>(1);
			auto minus = static_cast<ValueType>(-1);
			return _in > zero ? plus : _in < zero ? minus : zero;
		}

		inline Vector RotateVector(const Vector & _v, float _angle)
		{
			auto pi = acosf(-1);
			auto rad = -_angle * pi / 180.f;
			return Vector(_v.x * cosf(rad) - _v.y * sinf(rad), _v.x * sinf(rad) + _v.y * cosf(rad));
		}

		inline bool IntersectSegmentRectangle(const Vector & _point, const Vector & _direction, const Rectangle & _rect, Vector *_result = nullptr)
		{
			auto rotated_point = RotateVector(_point - _rect.position, _rect.angle);
			auto rotated_direction = RotateVector(_direction, _rect.angle);
			auto halfsize = Vector(_rect.size) / 2.f;

			auto scale = Vector(1.f) / rotated_direction;
			auto sign = Vector(Math::Sign(scale.x), Math::Sign(scale.y));
			auto near_p = (-sign * halfsize - rotated_point) * scale;
			auto far_p = (sign * halfsize - rotated_point) * scale;

			if (near_p.x > far_p.y || near_p.y > far_p.x)
			{
				return false;
			}

			float near_t = max(near_p.x, near_p.y);
			float far_t = min(far_p.x, far_p.y);

			if (near_t >= 1.f || far_t <= 0.f)
			{
				return false;
			}

			if (_result != nullptr)
			{
				if (near_t < 0.f)
				{
					near_t = 0.f;
				}

				*_result = _point + _direction * near_t;
			}

			return true;
		}

		inline bool IntersectRayCircle(const Vector & _point, const Vector & _direction, const Vector & _center, float _radius, Vector *_result = nullptr)
		{
			auto to_center = _center - _point;
			float projection_scale = DotProduct(_direction.GetNormalized(), to_center);
			auto projection = _point + _direction.GetNormalized() * projection_scale;
			float distance = (projection - _center).Length();

			if (distance < _radius)
			{
				if (_result != nullptr)
				{
					*_result = _point + _direction.GetNormalized() * (projection_scale - sqrtf(_radius * _radius - distance * distance));
				}
				return true;
			}

			return false;
		}
	}
}
