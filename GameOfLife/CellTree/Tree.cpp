#include "stdafx.h"

#include "Tree.h"

#include "Quad.h"

#include <chrono>

namespace CellTree
{
	Tree::Tree()
		: myRoot(nullptr)
		, myGeneration(0)
		, myEvolutionTime(0)
	{
		Point topLeft(-Constants::HalfSize, Constants::HalfSize);
		Point bottomRight(Constants::HalfSize, -Constants::HalfSize);

		myRoot = new Quad<Constants::MaxBitDepth>(this, nullptr, nullptr, topLeft, bottomRight);
		myRoot->SetRoot(myRoot);
	}

	Tree::~Tree()
	{
		SAFE_DELETE(myRoot);
	}

	void Tree::SetCellState(const Point& aPoint, const bool anIsAlive)
	{
		CellTree::Point topLeft;
		CellTree::Point bottomRight;
		myRoot->GetBounds(topLeft, bottomRight);
		if (Utils::ContainsPoint(aPoint, topLeft, bottomRight))
		{
			myRoot->SetCellState(aPoint, anIsAlive);
		}
	}

	// TODO: lower level quads should call up to their root instead of the tree,
	// they already notify the tree about needing to draw, but the SetCellState
	// and AddNeighbors interfaces are slow now with the bounds checking
	void Tree::AddNeighbors(const Point& aPoint, int aNumNeighbors)
	{
		CellTree::Point topLeft;
		CellTree::Point bottomRight;
		myRoot->GetBounds(topLeft, bottomRight);
		if (Utils::ContainsPoint(aPoint, topLeft, bottomRight))
		{
			myRoot->AddNeighbors(aPoint, aNumNeighbors);
		}
	}

	void Tree::Evolve()
	{
		using namespace std::chrono;
		high_resolution_clock::time_point start = high_resolution_clock::now();

		myLastUpdatedCellCount = myUpdatedCells.size();
		myUpdatedCells.clear();

		if (myRoot)
		{
			myRoot->PreEvolve();
			myRoot->Evolve();
			myRoot->PostEvolve();
			myGeneration++;
		}

		high_resolution_clock::time_point end = high_resolution_clock::now();

		myEvolutionTime = duration_cast<milliseconds>(end - start).count();
	}

	void Tree::GetBounds(Point& anOutTopLeft, Point& anOutBottomRight) const
	{
		myRoot->GetBounds(anOutTopLeft, anOutBottomRight);
	}

	void Tree::GetQuads(std::vector<PointPair>& someOutPairs) const
	{
		myRoot->GetQuads(someOutPairs);
	}

	void Tree::RefreshCells()
	{
		myRoot->RefreshCells();
	}
}