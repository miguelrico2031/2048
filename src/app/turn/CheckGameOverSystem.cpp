#include "app/turn/CheckGameOverSystem.h"

#include "app/board/BoardComponents.h"
#include "app/board/BoardHelpers.h"
#include "app/session/SessionComponents.h"
#include "app/turn/TurnComponents.h"
#include "core/helpers/EnttHelpers.h"
#include <entt/entt.hpp>
#include <iostream>

void ttfe::turn::CheckGameOverSystem(entt::registry& registry, float)
{
	auto turnView = registry.view<ttfe::turn::CheckingGameOverComponent>();
	if (turnView.empty())
		return;
	auto turnEntity = turnView.front();
	registry.remove<ttfe::turn::CheckingGameOverComponent>(turnEntity);

	const auto sessionView = registry.view<const ttfe::session::DataComponent>();
	const auto& sessionData = sessionView.get<const ttfe::session::DataComponent>(sessionView.front());

	const int maxTiles = sessionData.m_BoardRows * sessionData.m_BoardColumns;

	const auto tilesView = registry.view<const ttfe::board::TileComponent>();

	if (tilesView.size() == maxTiles)
	{
		ttfe::board::BoardData board{ sessionData.m_BoardRows, sessionData.m_BoardColumns };
		for (auto entity : tilesView)
		{
			const auto& tile = tilesView.get<const ttfe::board::TileComponent>(entity);
			ttfe::board::AddTileToBoard(board, { tile.m_Tile, entity });
		}
		if (ttfe::board::GetAvailableMoveDirections(board) == 0)
		{
			std::cout << "Game Over\n";
			return;
		}
	}
	entt::add_component<ttfe::turn::ExpectingActionComponent>(registry, turnEntity);
}
