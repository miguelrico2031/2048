#pragma once

#include "app/board/BoardDefinitions.h"
#include <raylib.h>
#include <vector>

namespace ttfe::session
{
	struct IsActiveComponent { struct Added {}; };

	struct DataComponent
	{
		int m_BoardRows = 4;
		int m_BoardColumns = 4;
		float m_BoardTilesSize = 64.f;
		float m_BoardTilesPadding = 4.f;
		float m_MoveSpeed = 1000.f;
		std::vector<ttfe::board::TileNumber> m_TileNumberBag = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 4 };
		Vector2 m_BoardCenter = { 0.f, 0.f };
		Color m_TileColor = MAROON;
		Color m_BackgroundTileColor = BROWN;
	};
}