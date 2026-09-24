#include "app/board/BoardHelpers.h"

#include "app/board/BoardDefinitions.h"
#include "app/session/SessionComponents.h"
#include <cassert>

Vector2 ttfe::board::BoardToWorld(const ttfe::board::Coords& coords, const ttfe::session::DataComponent& sessionData)
{
	const float step = sessionData.m_BoardTilesSize + sessionData.m_BoardTilesPadding;
    return {
        sessionData.m_BoardCenter.x + (coords.m_Col - (sessionData.m_BoardColumns - 1) * 0.5f) * step,

        sessionData.m_BoardCenter.y + (coords.m_Row - (sessionData.m_BoardRows - 1) * 0.5f) * step
    };
}

size_t ttfe::board::GetTileIndex(const ttfe::board::BoardData& board, ttfe::board::Coords coords)
{
    return ttfe::board::IsInBounds(coords.m_Row, board.m_Size.m_Row) && ttfe::board::IsInBounds(coords.m_Col, board.m_Size.m_Col)
        ? static_cast<size_t>(coords.m_Row * board.m_Size.m_Col + coords.m_Col)
        : board.m_Tiles.size();
}

void ttfe::board::AddTileToBoard(ttfe::board::BoardData& board, ttfe::board::TileData&& tile)
{
    size_t index = ttfe::board::GetTileIndex(board, tile.m_Tile.m_Coords);
    assert(board.m_Tiles[index] == std::nullopt);
    board.m_Tiles[index] = std::move(tile);
}

void ttfe::board::MoveTileInBoard(ttfe::board::BoardData& board, ttfe::board::Coords from, ttfe::board::Coords to)
{
	size_t fromIndex = ttfe::board::GetTileIndex(board, from);
	size_t toIndex = ttfe::board::GetTileIndex(board, to);
	assert(board.m_Tiles[fromIndex].has_value());
	assert(!board.m_Tiles[toIndex].has_value());
	board.m_Tiles[toIndex] = std::move(board.m_Tiles[fromIndex]);
	board.m_Tiles[fromIndex] = std::nullopt;
	board.m_Tiles[toIndex]->m_Tile.m_Coords = to;
}

std::vector<ttfe::board::Coords> ttfe::board::GetNeighbors(const ttfe::board::BoardData& board, ttfe::board::Coords coords)
{
	std::vector<ttfe::board::Coords> neighbors;
	neighbors.reserve(4);
	constexpr int rows[] = { -1, 1, 0, 0 };
	constexpr int cols[] = { 0, 0, -1, 1 };
	for (size_t i = 0; i < 4; i++)
	{
		const ttfe::board::Coords neighborCoords{ coords.m_Row + rows[i], coords.m_Col + cols[i] };
		const size_t index = ttfe::board::GetTileIndex(board, neighborCoords);
		if (IsInBounds(index, board.m_Tiles.size()))
		{
			neighbors.push_back(neighborCoords);
		}
	}
	return neighbors;
}

ttfe::move::Directions ttfe::board::GetAvailableMoveDirections(const ttfe::board::BoardData& board)
{
	ttfe::move::Directions directions = 0;
	for (int row = 0; row < board.m_Size.m_Row; row++)
	{
		for (int col = 0; col < board.m_Size.m_Col; col++)
		{
			const ttfe::board::Coords coords{ row, col };
			size_t index = ttfe::board::GetTileIndex(board, coords);
			if (!board.m_Tiles[index].has_value())
				continue;
			for (ttfe::board::Coords neighborCoords : ttfe::board::GetNeighbors(board, coords))
			{
				const size_t neighborIndex = ttfe::board::GetTileIndex(board, neighborCoords);
				const bool canMoveToNeighbor =
					!board.m_Tiles[neighborIndex].has_value() ||
					board.m_Tiles[neighborIndex]->m_Tile.m_Number == board.m_Tiles[index]->m_Tile.m_Number;

				if (!canMoveToNeighbor)
					continue;

				if (coords.m_Row < neighborCoords.m_Row)
				{
					directions |= static_cast<ttfe::move::Directions>(ttfe::move::Direction::Down);
				}
				else if (coords.m_Row > neighborCoords.m_Row)
				{
					directions |= static_cast<ttfe::move::Directions>(ttfe::move::Direction::Up);
				}
				else if (coords.m_Col < neighborCoords.m_Col)
				{
					directions |= static_cast<ttfe::move::Directions>(ttfe::move::Direction::Right);
				}
				else
				{
					directions |= static_cast<ttfe::move::Directions>(ttfe::move::Direction::Left);
				}
			}

		}
	}
	return directions;
}
