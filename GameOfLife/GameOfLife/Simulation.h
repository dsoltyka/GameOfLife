#pragma once

#include "../CellTree/Point.h"

#include <string>
#include <unordered_set>

namespace CellTree
{
	class Tree;
}

namespace GameOfLife
{
	class Simulation
	{
	public:

		static Simulation* GetInstance()
		{
			static Simulation* theInstance = nullptr;
			if (!theInstance)
				theInstance = new Simulation();

			return theInstance;
		}

		void Init(const std::string& anInputFile);

		void Update();

		void CreateGliderGunAt(const CellTree::Point& aPoint);
		void CreateOscillatorAt(const CellTree::Point& aPoint);

		const CellTree::Tree* GetTree() const { return myTree; }
		CellTree::Tree* GetTree() { return myTree; }

	private:
		Simulation();
		~Simulation();

		CellTree::Tree* myTree;

		std::unordered_set<CellTree::Point, CellTree::Point::Hash> myPendingAdds;
	};
}
