#pragma once

namespace ttfe::board
{
	using TileNumber = uint64_t;

	struct Tile
	{
		ttfe::board::TileNumber m_Number;
		int m_Row;
		int m_Col;
	};

	enum class Direction : uint8_t
	{
		Up,
		Down,
		Left,
		Right
	};
}