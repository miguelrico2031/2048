#pragma once

namespace ttfe::move
{
	enum class Direction : uint8_t
	{
		Up = 1 << 0,
		Down = 1 << 1,
		Left = 1 << 2,
		Right = 1 << 3
	};

	using Directions = uint8_t;

	inline const char* Debug_DirectionToString(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up: return "Up";
		case Direction::Down: return "Down";
		case Direction::Left: return "Left";
		case Direction::Right: return "Right";
		default: return nullptr;
		}
	}
}