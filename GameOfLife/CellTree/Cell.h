#pragma once

namespace CellTree
{
	struct Cell
	{
		Cell() : myNeighbors(0), myIsAlive(false) {}
		char myNeighbors;
		bool myIsAlive : 1;
	};
}