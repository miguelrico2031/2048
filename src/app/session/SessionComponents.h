#pragma once

#include "app/board/BoardDefinitions.h"
#include <raylib.h>
#include <vector>

namespace ttfe::session
{
	struct IsActiveComponent { struct Added {}; };

	struct DataComponent
	{
		int m_BoardRows = 0;
		int m_BoardColumns = 0;
		float m_BoardTilesSize = 0.f;
		float m_BoardTilesPadding = 0.f;
		Vector2 m_BoardCenter = { 0.f, 0.f };
		int m_InitialTilesCount = 0;
		std::vector<ttfe::board::TileNumber> m_TileNumberBag;
	};

	struct NewTurnEvent {};
}