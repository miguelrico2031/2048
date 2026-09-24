#pragma once
#include "app/board/BoardDefinitions.h"
#include <entt/entity/fwd.hpp>

namespace ttfe::board
{
	struct BackgroundTileComponent {};

	struct TileComponent
	{
		ttfe::board::Tile m_Tile;
		struct Added {};
	};

	struct CreateNewTileRequestEvent
	{
		ttfe::board::Tile m_Tile;
	};

	struct CreateMergedTileRequestEvent
	{
		ttfe::board::Tile m_Tile;
	};

	struct MergingTileComponent
	{
		entt::entity m_Other;
	};

	struct DestroyTileRequestComponent {};

	struct UpdateTileCoordsRequestComponent
	{
		ttfe::board::Coords m_Coords;
	};
}