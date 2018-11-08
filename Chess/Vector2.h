#ifndef VECTOR2_H
#define VECTOR2_H
#pragma once

template<typename T>
class Vec2D
{
	public:
	// fields
		T x;
		T y;
	// constructors/destructors
		Vec2D() = default;
        Vec2D(T _x, T _y) : x(_x), y(_y)
		{}
	// overloaded operators
		bool operator == (const Vec2D<T>& rhs) const
		{
			return (x == rhs.x && y == rhs.y);
		}
		bool operator != (const Vec2D<T>& rhs) const
		{
			return !(*this == rhs);
		}
		Vec2D<T> operator + (const Vec2D<T>& rhs) const
		{
			Vec2D<T> result;
			result.x = x + rhs.x;
			result.y = y + rhs.y;
			return result;
		}
		Vec2D<T> operator - (const Vec2D<T>& rhs) const
		{
			Vec2D<T> result;
			result.x = x - rhs.x;
			result.y = y - rhs.y;
			return result;
		}
		Vec2D<T>& operator - ()
		{
			x *= -1;
			y *= -1;
			return *this;
		}
		const Vec2D<T>& operator += (const Vec2D<T>& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}const Vec2D<T>& operator -= (const Vec2D<T>& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}
};

typedef Vec2D<int> Vector2;



#endif // VECTOR2_H
