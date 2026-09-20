#include "app/board/BoardHelpers.h"

#include "app/session/SessionComponents.h"

Vector2 ttfe::board::BoardToWorld(int row, int col, const ttfe::session::DataComponent& sessionData)
{
	const float step = sessionData.m_BoardTilesSize + sessionData.m_BoardTilesPadding;
	return {
		sessionData.m_BoardCenter.x + (col - (sessionData.m_BoardColumns - 1) * 0.5f) * step,
		sessionData.m_BoardCenter.y + (row - (sessionData.m_BoardRows - 1) * 0.5f) * step
	};
}