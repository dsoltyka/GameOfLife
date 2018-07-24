#pragma once

#include "Constants.h"
#include "Point.h"

#include <vector>
#include <unordered_map>

namespace CellTree
{
	template<int>
	class Quad;

	class Tree
	{
	public:

		Tree();
		~Tree();

		void SetCellState(const Point& aPoint, const bool anIsAlive);

		void AddNeighbors(const Point& aPoint, int aNumNeighbors);

		void Evolve();

		void GetBounds(Point& anOutTopLeft, Point& anOutBottomRight) const;

		void GetQuads(std::vector<PointPair>& someOutPairs) const;

		void RefreshCells();

		void NotifyCellUpdated(const Point& aPoint, const bool anIsAlive)
		{
			myUpdatedCells[aPoint] = anIsAlive;
		}

		const std::unordered_map<Point, bool, Point::Hash>& GetUpdatedCells() const { return myUpdatedCells; }

		uint64_t GetGeneration() const { return myGeneration; }
		uint64_t GetEvolutionTime() const { return myEvolutionTime; }
		size_t GetLastUpdatedCellCount() const { return myLastUpdatedCellCount; }

	private:
		Quad<Constants::MaxBitDepth>* myRoot;
		uint64_t myGeneration;
		uint64_t myEvolutionTime;
		size_t myLastUpdatedCellCount;

		std::unordered_map<Point, bool, Point::Hash> myUpdatedCells;
	};
}
