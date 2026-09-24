#pragma once 

#include "app/board/BoardDefinitions.h"
#include "app/movement/MovementDefinitions.h"
#include <raylib.h>

namespace ttfe::move
{
	struct BoardMovementRequestEvent
	{
		ttfe::move::Direction m_Direction;
	};

	struct MoveTileRequestComponent
	{
		ttfe::board::Coords m_Target;
	};

	struct IsMovingComponent
	{
		Vector2 m_Movement;
		Vector2 m_TargetPosition;
		ttfe::board::Coords m_TargetCoords;
	};
}