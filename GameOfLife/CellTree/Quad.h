#pragma once

#include "Cell.h"
#include "Constants.h"
#include "Tree.h"
#include "Utils.h"

#include <cassert>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace CellTree
{
#ifdef _DEBUG
	static uintptr_t ourLastLeafToAddNeighbors;
#endif

	typedef Quad<Constants::MaxBitDepth> RootQuad;

	template <int BitDepth>
	class Quad
	{
		static const int ourBitDepth = BitDepth;
		typedef Quad<(ourBitDepth - 1)> ChildQuad;
		typedef Quad<(ourBitDepth + 1)> ParentQuad;

	public:

		Quad(Tree* aTree, RootQuad* aRoot, ParentQuad* aParent, const Point& aTopLeft, const Point& aBottomRight)
			: myTree(aTree)
			, myRoot(aRoot)
			, myParent(aParent)
			, myTopLeft(aTopLeft)
			, myBottomRight(aBottomRight)
			, myNw(nullptr)
			, myNe(nullptr)
			, mySw(nullptr)
			, mySe(nullptr)
		{
		}

		~Quad()
		{
			SAFE_DELETE(myNw);
			SAFE_DELETE(myNe);
			SAFE_DELETE(mySw);
			SAFE_DELETE(mySe);
		}

		void SetRoot(RootQuad* aRoot) { myRoot = aRoot; }

		void SetCellState(const Point& aPoint, const bool anIsAlive)
		{
			ChildQuad* child = GetOrCreateChildForPoint(aPoint);
			child->SetCellState(aPoint, anIsAlive);
		}

		void AddNeighbors(const Point& aPoint, int aNumNeighbors)
		{
			ChildQuad* child = GetOrCreateChildForPoint(aPoint);
			child->AddNeighbors(aPoint, aNumNeighbors);
		}

		void RefreshCells()
		{
			if (myNw)
				myNw->RefreshCells();

			if (myNe)
				myNe->RefreshCells();

			if (mySw)
				mySw->RefreshCells();

			if (mySe)
				mySe->RefreshCells();
		}

		bool ContainsCells() const
		{
			if (myNw && myNw->ContainsCells())
				return true;

			if (myNe && myNe->ContainsCells())
				return true;

			if (mySw && mySw->ContainsCells())
				return true;

			if (mySe && mySe->ContainsCells())
				return true;

			return false;
		}

		void PreEvolve()
		{
			if (myNw)
				myNw->PreEvolve();

			if (myNe)
				myNe->PreEvolve();

			if (mySw)
				mySw->PreEvolve();

			if (mySe)
				mySe->PreEvolve();
		}

		void Evolve()
		{
			// Evolution may cause the creation of new children, which we
			// do not want to include in this generation of evolution
			ChildQuad* tempNw = myNw;
			ChildQuad* tempNe = myNe;
			ChildQuad* tempSw = mySw;
			ChildQuad* tempSe = mySe;

			if (tempNw)
				tempNw->Evolve();

			if (tempNe)
				tempNe->Evolve();

			if (tempSw)
				tempSw->Evolve();

			if (tempSe)
				tempSe->Evolve();
		}

		void PostEvolve()
		{
			if (myNw)
			{
				if (myNw->ContainsCells())
				{
					myNw->PostEvolve();
				}
				else
				{
					SAFE_DELETE(myNw);
				}
			}

			if (myNe)
			{
				if (myNe->ContainsCells())
				{
					myNe->PostEvolve();
				}
				else
				{
					SAFE_DELETE(myNe);
				}
			}

			if (mySw)
			{
				if (mySw->ContainsCells())
				{
					mySw->PostEvolve();
				}
				else
				{
					SAFE_DELETE(mySw);
				}
			}

			if (mySe)
			{
				if (mySe->ContainsCells())
				{
					mySe->PostEvolve();
				}
				else
				{
					SAFE_DELETE(mySe);
				}
			}
		}

		void GetQuads(std::vector<PointPair>& someOutPairs) const
		{
			someOutPairs.push_back(PointPair(myTopLeft, myBottomRight));

			if (myNw)
			{
				myNw->GetQuads(someOutPairs);
			}

			if (myNe)
			{
				myNe->GetQuads(someOutPairs);
			}

			if (mySw)
			{
				mySw->GetQuads(someOutPairs);
			}

			if (mySe)
			{
				mySe->GetQuads(someOutPairs);
			}
		}

		void GetBounds(Point& anOutTopLeft, Point& anOutBottomRight) const
		{
			anOutTopLeft = myTopLeft;
			anOutBottomRight = myBottomRight;
		}

	private:

		ChildQuad* GetOrCreateChildForPoint(const Point& aPoint)
		{
			ChildQuad* child = nullptr;

			int64_t halfWidth;
			int64_t halfHeight;
			int64_t widthAdjust;
			int64_t heightAdjust;
			Utils::GetChildHalfDimensions<BitDepth>(myTopLeft, myBottomRight, halfWidth, halfHeight, widthAdjust, heightAdjust);

			// West
			if (aPoint.myX <= (myBottomRight.myX - halfWidth - widthAdjust))
			{
				// North
				if (aPoint.myY >= (myBottomRight.myY + halfHeight))
				{
					if (!myNw)
					{
						const Point bottomRight((myBottomRight.myX - halfWidth - widthAdjust), (myBottomRight.myY + halfHeight));
						myNw = new ChildQuad(myTree, myRoot, this, myTopLeft, bottomRight);
					}

					child = myNw;
				}
				// South
				else
				{
					if (!mySw)
					{
						const Point topLeft(myTopLeft.myX, (myTopLeft.myY - halfHeight - heightAdjust));
						const Point bottomRight((myBottomRight.myX - halfWidth - widthAdjust), myBottomRight.myY);

						mySw = new ChildQuad(myTree, myRoot, this, topLeft, bottomRight);
					}

					child = mySw;
				}
			}
			// East
			else
			{
				// North
				if (aPoint.myY >= (myBottomRight.myY + halfHeight))
				{
					if (!myNe)
					{
						const Point topLeft((myTopLeft.myX + halfWidth), myTopLeft.myY);
						const Point bottomRight(myBottomRight.myX, (myBottomRight.myY + halfHeight));
						myNe = new ChildQuad(myTree, myRoot, this, topLeft, bottomRight);
					}

					child = myNe;
				}
				// South
				else
				{
					if (!mySe)
					{
						const Point topLeft((myTopLeft.myX + halfWidth), (myTopLeft.myY - halfHeight - heightAdjust));
						mySe = new ChildQuad(myTree, myRoot, this, topLeft, myBottomRight);
					}

					child = mySe;
				}
			}

			return child;
		}

		Tree* myTree;
		RootQuad* myRoot;
		ParentQuad* myParent;

		Point myTopLeft;
		Point myBottomRight;

		// Children
		ChildQuad* myNw;
		ChildQuad* myNe;
		ChildQuad* mySw;
		ChildQuad* mySe;
	};

	template <>
	class Quad<Constants::MinBitDepth>
	{
		typedef Quad<(Constants::MinBitDepth + 1)> ParentQuad;

	public:

		Quad(Tree* aTree, RootQuad* aRoot, ParentQuad* aParent, const Point& aTopLeft, const Point& aBottomRight)
			: myTree(aTree)
			, myRoot(aRoot)
			, myParent(aParent)
			, myTopLeft(aTopLeft)
			, myBottomRight(aBottomRight)
		{
			myWidth = std::abs(myBottomRight.myX - myTopLeft.myX) + 1;
			myHeight = std::abs(myBottomRight.myY - myTopLeft.myY) + 1;

			myActiveCells = new Cell[myWidth * myHeight];
			myTempCells = new Cell[myWidth * myHeight];
		}

		~Quad()
		{
			delete[] myActiveCells;
			delete[] myTempCells;
		}

		void SetRoot(RootQuad* aRoot) { myRoot = aRoot; }

		void SetCellState(const Point& aPoint, const bool anIsAlive)
		{
			const int64_t cellIdx = GetCellIndex(aPoint);
			myActiveCells[cellIdx].myIsAlive = anIsAlive;

			myTree->NotifyCellUpdated(aPoint, anIsAlive);

			const int neighborAccum = anIsAlive ? 1 : -1;
			for (int oy = -1; oy < 2; ++oy)
			{
				for (int ox = -1; ox < 2; ++ox)
				{
					Point neighbor;
					neighbor.myX = aPoint.myX + ox;
					neighbor.myY = aPoint.myY + oy;

#if MAX_BIT_DEPTH == 64
					// If we wrap around to (1 << 63), we need to move a bit more to correctly
					// overflow into the opposite signed region.  This can't be done in the utility
					// method as we need to know in which direction we are trying to offset.
					const int64_t bound = (1LL << 63);
					if (neighbor.myX == bound)
					{
						if (ox < 0)
							neighbor.myX--;
						else if (ox > 0)
							neighbor.myX++;
					}

					if (neighbor.myY == bound)
					{
						if (oy < 0)
							neighbor.myY--;
						else if (oy > 0)
							neighbor.myY++;
					}
#else
					Utils::WrapCoordinates(neighbor);
#endif

					if (neighbor == aPoint)
						continue;

					if (Utils::ContainsPoint(neighbor, myTopLeft, myBottomRight))
					{
#ifdef _DEBUG
						// TODO:  There is currently a bug with the subdivision of a 64 bit quad tree
						// where leafs may be created with bounds that overlap by 1 unit.  This causes
						// destabilization in the simulation as well as precludes a nice optimization
						// opportunity here as we really shouldn't need to go up to the root if the point
						// is local.  Curiously, the simulation remains (ostensibly) stable when we
						// change cell state locally, it's only neighbor state that causes issues.
						ourLastLeafToAddNeighbors = (uintptr_t)this;
						myRoot->AddNeighbors(neighbor, neighborAccum);
						ourLastLeafToAddNeighbors = 0;
#else
						myRoot->AddNeighbors(neighbor, neighborAccum);
#endif
					}
					else
					{
						myRoot->AddNeighbors(neighbor, neighborAccum);
					}
				}
			}
		}

		void AddNeighbors(const Point& aPoint, int aNumNeighbors)
		{
#ifdef _DEBUG
			// See comments within SetCellState.  This allows us to trap the offending
			// overlapping leafs.
			if (ourLastLeafToAddNeighbors != 0 && ourLastLeafToAddNeighbors != uintptr_t(this))
			{
				//assert(false);
			}
#endif

			const int64_t cellIdx = GetCellIndex(aPoint);
			myActiveCells[cellIdx].myNeighbors += aNumNeighbors;
		}

		void RefreshCells()
		{
			for (int y = 0; y < myHeight; ++y)
			{
				for (int x = 0; x < myWidth; ++x)
				{
					Point p = myTopLeft + Point(x, -y);
					const int64_t cellIdx = GetCellIndex(p);
					const Cell& cell = myActiveCells[cellIdx];

					if (cell.myIsAlive)
						myTree->NotifyCellUpdated(p, true);
				}
			}
		}

		bool ContainsCells() const
		{
			for (int y = 0; y < myHeight; ++y)
			{
				for (int x = 0; x < myWidth; ++x)
				{
					Point p = myTopLeft + Point(x, -y);
					const int64_t cellIdx = GetCellIndex(p);
					const Cell& cell = myActiveCells[cellIdx];

					if (cell.myIsAlive || cell.myNeighbors > 0)
						return true;
				}
			}

			return false;
		}

		void PreEvolve()
		{
			memcpy(myTempCells, myActiveCells, sizeof(Cell) * (myWidth * myHeight));
		}

		void Evolve()
		{
			for (int y = 0; y < myHeight; ++y)
			{
				for (int x = 0; x < myWidth; ++x)
				{
					UpdateCellState(myTopLeft + Point(x, -y));
				}
			}
		}

		void PostEvolve() { }

		void GetQuads(std::vector<PointPair>& someOutPairs) const
		{
			someOutPairs.push_back(PointPair(myTopLeft, myBottomRight));
		}

		void GetBounds(Point& anOutTopLeft, Point& anOutBottomRight) const
		{
			anOutTopLeft = myTopLeft;
			anOutBottomRight = myBottomRight;
		}

	private:

		int64_t GetCellIndex(const Point& aPoint) const
		{
			const Point local = aPoint - myTopLeft;
			const int64_t idx = (std::abs(local.myY) * myWidth) + std::abs(local.myX);
			return idx;
		}

		void UpdateCellState(const Point& aPoint)
		{
			const int64_t cellIdx = GetCellIndex(aPoint);
			const int neighborCount = myTempCells[cellIdx].myNeighbors;

			// Apply basic Conway B3/S23 rule
			if (myTempCells[cellIdx].myIsAlive)
			{
				if (neighborCount != 2 && neighborCount != 3)
				{
					if (Utils::ContainsPoint(aPoint, myTopLeft, myBottomRight))
					{
						SetCellState(aPoint, false);
					}
					else
					{
						myRoot->SetCellState(aPoint, false);
					}
				}
			}
			else
			{
				if (neighborCount == 3)
				{
					if (Utils::ContainsPoint(aPoint, myTopLeft, myBottomRight))
					{
						SetCellState(aPoint, true);
					}
					else
					{
						myRoot->SetCellState(aPoint, true);
					}
				}
			}
		}

		Tree* myTree;
		RootQuad* myRoot;
		ParentQuad* myParent;
		Point myTopLeft;
		Point myBottomRight;

		int64_t myWidth;
		int64_t myHeight;

		Cell* myActiveCells;
		Cell* myTempCells;
	};
}