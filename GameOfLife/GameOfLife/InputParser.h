#pragma once

#include "../CellTree/Point.h"

#include <list>
#include <string>

namespace GameOfLife
{
	class InputParser
	{
	public:
		InputParser();
		~InputParser();

		bool ParseInput(const std::string& aFileName);

		const std::list<CellTree::Point>& GetParsedInput() const { return myParsedInput; }

	private:
		std::list<CellTree::Point> myParsedInput;
	};
}
