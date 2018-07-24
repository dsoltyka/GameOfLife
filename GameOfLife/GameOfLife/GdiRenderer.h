#pragma once

#include "../CellTree/Point.h"

#include <windows.h>

namespace GameOfLife
{
	class GdiRenderer
	{
	public:
		static GdiRenderer* GetInstance()
		{
			static GdiRenderer* theInstance = nullptr;
			if (!theInstance)
				theInstance = new GdiRenderer();

			return theInstance;
		}

		~GdiRenderer();

		void Render(HDC aHdc);

		void AdjustTopLeft(const CellTree::Point& anAdjustment)
		{
			SetTopLeft(myTopLeft + anAdjustment);
			myRequiresClear = true;
		}

		void AdjustPixelSize(const int16_t anAdjustment)
		{
			myPixelSize += anAdjustment;
			myPixelSize = max(1U, min(myPixelSize, 6U));
			myRequiresClear = true;
		}

		void SetRenderBounds(const int aWidth, const int aHeight, const bool aCenterView = true);

		const CellTree::Point& GetTopLeft() const { return myTopLeft; }
		const uint16_t GetPixelSize() const { return myPixelSize; }

	private:

		void SetTopLeft(const CellTree::Point& aTopLeft) { myTopLeft = aTopLeft; }

		bool IsWithinRenderBounds(const CellTree::Point& aPoint) const
		{
			if ((aPoint.myX < 0) ||
				(aPoint.myX > myRenderBounds.myX) ||
				(aPoint.myY < 0) ||
				(aPoint.myY > myRenderBounds.myX))
			{
				return false;
			}

			return true;
		}

		GdiRenderer();

		bool myRequiresClear;
		CellTree::Point myDrawOffset;

		CellTree::Point myTopLeft;
		CellTree::Point myRenderBounds;

		uint16_t myPixelSize;
	};
}
