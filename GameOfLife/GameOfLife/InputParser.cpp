#include "stdafx.h"
#include "InputParser.h"

#include <fstream>

namespace GameOfLife
{
	InputParser::InputParser()
	{
	}

	InputParser::~InputParser()
	{
	}

	bool InputParser::ParseInput(const std::string& aFileName)
	{
		myParsedInput.clear();

		std::ifstream input(aFileName);

		if (!input.is_open())
		{
			// Unable to open file
			return false;
		}

		char delim;
		while (true)
		{
			CellTree::Point point;
			input >> delim >> point.myX >> delim >> point.myY >> delim;
			if (input.eof())
				break;

			myParsedInput.push_back(point);
		}

		return true;
	}
}