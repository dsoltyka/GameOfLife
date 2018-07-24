#pragma once

#include <stdint.h>

#define MAX_BIT_DEPTH 64
#define MIN_BIT_DEPTH 3

static_assert(MAX_BIT_DEPTH >= 1 && MAX_BIT_DEPTH <= 64, "Invalid Max Bit Depth");
static_assert(MIN_BIT_DEPTH >= 1 && MIN_BIT_DEPTH <= 64, "Invalid Min Bit Depth");

// Disable the following warning as we are purposefully overflowing

// warning C4307: '-': integral constant overflow
#pragma warning( push )
#pragma warning(disable:4307)

namespace CellTree
{
	namespace Constants
	{
		static const int MaxBitDepth = MAX_BIT_DEPTH;
		static const int MinBitDepth = MIN_BIT_DEPTH;

		static const int64_t HalfSize = (1LL << (MAX_BIT_DEPTH - 1)) - 1LL;
	}
}

#pragma warning( pop ) 