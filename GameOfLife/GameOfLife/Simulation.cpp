#include "stdafx.h"

#include "Simulation.h"

#include "InputParser.h"

#include "../CellTree/Tree.h"
#include "../CellTree/Constants.h"

#include <list>

namespace GameOfLife
{
	Simulation::Simulation()
		: myTree(new CellTree::Tree())
	{
	}

	Simulation::~Simulation()
	{
		SAFE_DELETE(myTree);
	}

	void Simulation::Init(const std::string& anInputFile)
	{
		InputParser parser;

		if (parser.ParseInput(anInputFile))
		{
			auto iter = parser.GetParsedInput().begin();
			auto endIter = parser.GetParsedInput().end();
			for (; iter != endIter; ++iter)
			{
				myTree->SetCellState(*iter, true);
			}
		}
	}

	void Simulation::Update()
	{
		auto iter = myPendingAdds.begin();
		for (iter; iter != myPendingAdds.end(); ++iter)
		{
			myTree->SetCellState(*iter, true);
		}
		myPendingAdds.clear();

		myTree->Evolve();
	}

	void Simulation::CreateGliderGunAt(const CellTree::Point& aPoint)
	{
		bool shape[11][38] = {
			{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
			{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false },
			{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, true,  false, false, false, false, false, false, false, false, false, false, false, false },
			{ false, false, false, false, false, false, false, false, false, false, false, false, false, true,  true,  false, false, false, false, false, false, true,  true,  false, false, false, false, false, false, false, false, false, false, false, false, true,  true,  false },
			{ false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false, true,  false, false, false, false, true,  true,  false, false, false, false, false, false, false, false, false, false, false, false, true,  true,  false },
			{ false, true,  true,  false, false, false, false, false, false, false, false, true,  false, false, false, false, false, true,  false, false, false, true,  true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
			{ false, true,  true,  false, false, false, false, false, false, false, false, true,  false, false, false, true,  false, true,  true,  false, false, false, false, true,  false, true,  false, false, false, false, false, false, false, false, false, false, false, false },
			{ false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false, true,  false, false, false, false, false, false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false },
			{ false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
			{ false, false, false, false, false, false, false, false, false, false, false, false, false, true,  true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
			{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
		};

		const int sx = ((double)rand() / (RAND_MAX)) > 0.5 ? -1 : 1;
		const int sy = ((double)rand() / (RAND_MAX)) > 0.5 ? -1 : 1;
		const bool invert = ((double)rand() / (RAND_MAX)) > 0.5 ? true : false;

		for (int y = 0; y < 11; ++y)
		{
			for (int x = 0; x < 38; ++x)
			{
				bool state = shape[y][x];
				if (state)
				{
					CellTree::Point p;
					if (invert)
						p = CellTree::Point(y * sy, x * sx);
					else
						p = CellTree::Point(x * sx, y * sy);

					myPendingAdds.emplace(aPoint + p);
				}
			}
		}
	}

	void Simulation::CreateOscillatorAt(const CellTree::Point& aPoint)
	{
		for (int x = 0; x < 3; ++x)
		{
			myPendingAdds.emplace(aPoint + CellTree::Point(x, 0));
		}
	}
}