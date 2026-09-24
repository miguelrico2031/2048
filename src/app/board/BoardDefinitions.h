#pragma once
#include <cstdint>

namespace ttfe::board
{
	using TileNumber = uint64_t;

	struct Coords
	{
		int m_Row;
		int m_Col;

		bool operator==(const Coords&) const = default;
	};

	struct Tile
	{
		ttfe::board::TileNumber m_Number;
		ttfe::board::Coords m_Coords;
		bool operator==(const Tile&) const = default;
	};
}