#pragma once

namespace CellTree
{
	struct Cell
	{
		Cell() : myIsAlive(false), myNeighbors(0) {}
		bool myIsAlive : 1;
		char myNeighbors;
	};
}