#pragma once

#include "app/board/BoardDefinitions.h"
#include "app/movement/MovementDefinitions.h"
#include <entt/entity/fwd.hpp>
#include <raylib.h>
#include <optional>
#include <vector> 


namespace ttfe::session
{
	struct DataComponent;
}
namespace ttfe::board
{

	struct TileData
	{
		ttfe::board::Tile m_Tile;
		entt::entity m_Entity;
	};

	struct BoardData
	{
		BoardData(int rows, int cols)
			: m_Tiles(static_cast<size_t>(rows * cols), std::nullopt)
			, m_Size(rows, cols) {}
		std::vector<std::optional<ttfe::board::TileData>> m_Tiles;
		const ttfe::board::Coords m_Size;
	};

	template<typename T, typename S>
	bool IsInBounds(T index, S size)
	{
		return index >= 0 && index < size;
	}

	Vector2 BoardToWorld(const ttfe::board::Coords& coords, const ttfe::session::DataComponent& sessionData);

	size_t GetTileIndex(const ttfe::board::BoardData& board, ttfe::board::Coords coords);

	void AddTileToBoard(ttfe::board::BoardData& board, ttfe::board::TileData&& tile);

	void MoveTileInBoard(ttfe::board::BoardData& board, ttfe::board::Coords from, ttfe::board::Coords to);

	std::vector<ttfe::board::Coords> GetNeighbors(const ttfe::board::BoardData& board, ttfe::board::Coords coords);

	ttfe::move::Directions GetAvailableMoveDirections(const ttfe::board::BoardData& board);

}