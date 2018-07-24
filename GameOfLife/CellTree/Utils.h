#pragma once

#include "Point.h"
#include "Constants.h"

namespace CellTree
{
	class Utils
	{
	public:
		static void WrapCoordinates(Point& anInOutPoint)
		{
#if MAX_BIT_DEPTH != 64
			const int64_t halfSize = (1LL << (int64_t(Constants::MaxBitDepth) - 1LL)) - 1LL;
			const int64_t adjustment = (halfSize * 2LL) + 1LL;

			while (anInOutPoint.myX < -halfSize) anInOutPoint.myX += adjustment;
			while (anInOutPoint.myX > halfSize) anInOutPoint.myX -= adjustment;
			while (anInOutPoint.myY < -halfSize) anInOutPoint.myY += adjustment;
			while (anInOutPoint.myY > halfSize) anInOutPoint.myY -= adjustment;
#endif
		}

		static bool ContainsPoint(const Point& aTestPoint, const Point& aTopLeft, const Point& aBottomRight)
		{
			if ((aTestPoint.myX < aTopLeft.myX) ||
				(aTestPoint.myX > aBottomRight.myX) ||
				(aTestPoint.myY > aTopLeft.myY) ||
				(aTestPoint.myY < aBottomRight.myY))
			{
				return false;
			}

			return true;
		}

		template <int BitDepth>
		static void GetChildHalfDimensions(const Point& aTopleft, const Point& aBottomRight, int64_t& anOutHalfWidth, int64_t& anOutHalfHeight, int64_t& anOutWidthAdjust, int64_t& anOutHeightAdjust)
		{
			anOutHalfWidth = std::abs(aTopleft.myX - aBottomRight.myX) + 1;
			anOutHalfHeight = std::abs(aTopleft.myY - aBottomRight.myY) + 1;

			// We need to subdivide unevenly if we have odd width or height
			{
				auto res = std::div(anOutHalfWidth, int64_t(2));
				anOutHalfWidth = res.quot;
				anOutWidthAdjust = res.rem;
			}
			{
				auto res = std::div(anOutHalfHeight, int64_t(2));
				anOutHalfHeight = res.quot;
				anOutHeightAdjust = res.rem;
			}
		}

		// We know our 64 bit bounds intrinsically as a 64 bit depth
		// can only represent the highest level of the tree
		template <>
		static void GetChildHalfDimensions<64>(const Point& /*aTopleft*/, const Point& /*aBottomRight*/, int64_t& anOutHalfWidth, int64_t& anOutHalfHeight, int64_t& anOutWidthAdjust, int64_t& anOutHeightAdjust)
		{
			anOutWidthAdjust = anOutHeightAdjust = 1LL;
			anOutHalfWidth = anOutHalfHeight = 0x7fffffffffffffff;
		}
	};
}