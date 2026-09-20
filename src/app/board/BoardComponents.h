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

	struct CreateTileRequestEvent
	{
		ttfe::board::Tile m_Tile;
	};

	struct DestroyTileRequestEvent
	{
		entt::entity m_TileEntity;
	};

	struct MoveTileRequestEvent
	{
		entt::entity m_TileEntity;
		ttfe::board::Direction m_Direction;
	};
}