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
		CellTree::Point point;
		while (true)
		{
			// We expect input data to be in (x, y) format
			input >> delim >> point.myX >> delim >> point.myY >> delim;
			if (input.eof())
				break;

			myParsedInput.push_back(point);
		}

		return true;
	}
}