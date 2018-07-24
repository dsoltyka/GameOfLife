#pragma once

#include <functional>

namespace CellTree
{
	struct Point
	{
		Point()
			: myX(0)
			, myY(0)
		{ }

		Point(const int64_t aX, const int64_t aY)
			: myX(aX)
			, myY(aY)
		{ }

		bool operator==(const Point& aRhs) const
		{
			return myX == aRhs.myX && myY == aRhs.myY;
		}

		bool operator!=(const Point& aRhs) const
		{
			return !(*this == aRhs);
		}

		Point& operator+=(const Point& aRhs)
		{
			myX += aRhs.myX;
			myY += aRhs.myY;

			return *this;
		}

		Point operator+(const Point& aRhs) const
		{
			Point ret(*this);
			ret += aRhs;
			return ret;
		}

		Point& operator-=(const Point& aRhs)
		{
			myX -= aRhs.myX;
			myY -= aRhs.myY;

			return *this;
		}

		Point operator-(const Point& aRhs) const
		{
			Point ret(*this);
			ret -= aRhs;
			return ret;
		}

		struct Hash
		{
			std::size_t operator() (const Point& aPoint) const
			{
				return std::hash<int64_t>()(aPoint.myX) ^ std::hash<int64_t>()(aPoint.myY);
			}
		};

		int64_t myX;
		int64_t myY;
	};

	typedef std::pair<Point, Point> PointPair;
}
