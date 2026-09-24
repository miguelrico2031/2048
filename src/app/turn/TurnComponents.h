#pragma once

namespace ttfe::turn
{
	struct SpawningTileComponent
	{
		size_t m_Tiles = 1;
	};
	struct CheckingGameOverComponent {};
	struct ExpectingActionComponent {};
	struct MovingTilesComponent {};
}