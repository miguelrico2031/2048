#include "app/movement/MoveBoardSystem.h"

#include "app/board/BoardComponents.h"
#include "app/board/BoardHelpers.h"
#include "app/movement/MovementComponents.h"
#include "app/session/SessionComponents.h"
#include "app/turn/TurnComponents.h"
#include "core/helpers/EnttHelpers.h"
#include <cassert>
#include <entt/entt.hpp>
#include <optional>
#include <unordered_map>
#include <vector>

namespace
{
	void DisplaceTile(
		ttfe::board::BoardData& board,
		std::unordered_map<entt::entity, entt::entity>& mergedTiles, 
		ttfe::board::Coords coords, ttfe::board::Coords direction)
	{
		const size_t index = ttfe::board::GetTileIndex(board, coords);
		if (!board.m_Tiles[index].has_value())
			return;

		bool canContinueMoving;
		do
		{
			canContinueMoving = false;
			const ttfe::board::Coords neighborCoords{ coords.m_Row + direction.m_Row, coords.m_Col + direction.m_Col };
			const size_t neighborIndex = ttfe::board::GetTileIndex(board, neighborCoords);
			if (ttfe::board::IsInBounds(neighborIndex, board.m_Tiles.size()))
			{
				auto& neighbor = board.m_Tiles[neighborIndex];
				if (!neighbor.has_value())
				{
					ttfe::board::MoveTileInBoard(board, coords, neighborCoords);
					canContinueMoving = true;
					coords = neighborCoords;
				}
				else if (mergedTiles.find(neighbor->m_Entity) == mergedTiles.end())
				{
					auto& tile = board.m_Tiles[ttfe::board::GetTileIndex(board, coords)];
					if (tile.has_value() && neighbor->m_Tile.m_Number == tile->m_Tile.m_Number)
					{
						mergedTiles[neighbor->m_Entity] = tile->m_Entity;
						tile = std::nullopt;
					}
				}
			}
		} while (canContinueMoving);
	}
}

void ttfe::move::MoveBoardSystem(entt::registry& registry, float)
{
	entt::clear_temp_components<ttfe::move::MoveTileRequestComponent>(registry);

	const auto eventView = registry.view<const ttfe::move::BoardMovementRequestEvent>();
	if (eventView.empty())
		return;

	auto turnView = registry.view<ttfe::turn::ExpectingActionComponent>();
	if (turnView.empty())
		return;

	const ttfe::move::Direction direction = eventView.get<const ttfe::move::BoardMovementRequestEvent>(eventView.front()).m_Direction;

	const auto sessionView = registry.view<const ttfe::session::DataComponent>();
	const auto& sessionData = sessionView.get<const ttfe::session::DataComponent>(sessionView.front());

	ttfe::board::BoardData board{ sessionData.m_BoardRows, sessionData.m_BoardColumns };

	auto tilesView = registry.view<ttfe::board::TileComponent>();
	for (auto entity : tilesView)
	{
		const auto& tileComponent = tilesView.get<const ttfe::board::TileComponent>(entity);
		ttfe::board::AddTileToBoard(board, { tileComponent.m_Tile, entity });
	}

	const ttfe::move::Directions availableDirections = ttfe::board::GetAvailableMoveDirections(board);

	if ((availableDirections & static_cast<ttfe::move::Directions>(direction)) == 0)
		return;

	std::unordered_map<entt::entity, entt::entity> mergedTiles;

	//Move Tiles

	#pragma warning(suppress: 26813)
	const ttfe::board::Coords moveDirection{
		direction == ttfe::move::Direction::Up ? -1 : (direction == ttfe::move::Direction::Down ? 1 : 0),
		direction == ttfe::move::Direction::Left ? -1 : (direction == ttfe::move::Direction::Right ? 1 : 0)
	};

	const bool positiveDirection = moveDirection.m_Row > 0 || moveDirection.m_Col > 0;
	const bool verticalDirection = moveDirection.m_Row != 0;

	const int lineSize = verticalDirection ? board.m_Size.m_Row : board.m_Size.m_Col;
	const int otherAxisSize = verticalDirection ? board.m_Size.m_Col : board.m_Size.m_Row;

	const int lineToStart = positiveDirection ? lineSize - 2 : 1;
	const int lineToEnd = positiveDirection ? 0 : lineSize - 1;

	{
		assert(ttfe::board::IsInBounds(lineToStart, lineSize) && ttfe::board::IsInBounds(lineToEnd, lineSize));
		const auto positiveValid = lineToStart >= lineToEnd;
		const auto negativeValid = lineToStart <= lineToEnd;
		assert((positiveDirection && positiveValid) || (!positiveDirection && negativeValid));
	}

	const int increment = positiveDirection ? -1 : 1;
	const auto isLineInBounds = [&](int line) -> bool { return positiveDirection ? line >= lineToEnd : line <= lineToEnd; };

	int currentLine = lineToStart;
	while (isLineInBounds(currentLine))
	{
		for (int otherAxis = 0; otherAxis < otherAxisSize; otherAxis++)
		{
			const ttfe::board::Coords coords{
				verticalDirection ? currentLine : otherAxis,
				verticalDirection ? otherAxis : currentLine,
			};
			DisplaceTile(board, mergedTiles, coords, moveDirection);
		}

		currentLine += increment;
	 }

	for (const auto& tileData : board.m_Tiles)
	{
		if (!tileData.has_value())
			continue;

		const auto& tileComponent = registry.get<const ttfe::board::TileComponent>(tileData->m_Entity);

		if (tileComponent.m_Tile.m_Coords != tileData->m_Tile.m_Coords)
		{
			auto& moveComponent = entt::add_component<ttfe::move::MoveTileRequestComponent>(registry, tileData->m_Entity);
			moveComponent.m_Target = tileData->m_Tile.m_Coords;
		}
		// :{v
		// <]:3-|--<
		auto it = mergedTiles.find(tileData->m_Entity);
		if (it != mergedTiles.end())
		{
			auto merged = it->second;
			auto& moveComponent = entt::add_component<ttfe::move::MoveTileRequestComponent>(registry, merged);
			moveComponent.m_Target = tileData->m_Tile.m_Coords;

			entt::add_component<ttfe::board::MergingTileComponent>(registry, tileData->m_Entity).m_Other = merged;
			entt::add_component<ttfe::board::MergingTileComponent>(registry, merged).m_Other = tileData->m_Entity;
		}
	}

	auto turnEntity = turnView.front();
	registry.remove<ttfe::turn::ExpectingActionComponent>(turnEntity);
	entt::add_component<ttfe::turn::MovingTilesComponent>(registry, turnEntity);
}