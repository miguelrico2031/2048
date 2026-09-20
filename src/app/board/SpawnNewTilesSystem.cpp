#include "app/board/SpawnNewTilesSystem.h"

#include "app/board/BoardComponents.h"
#include "app/session/SessionComponents.h"
#include "core/helpers/EnttHelpers.h"
#include <cassert>
#include <entt/entt.hpp>
#include <random>
#include <raylib.h>
#include <utility>
#include <vector>

namespace
{
	std::random_device s_RandomDevice;
	std::mt19937 s_RandomGen{ s_RandomDevice() };

	std::vector<std::pair<int, int>> FindAvailableBoardPositions(entt::registry& registry, const ttfe::session::DataComponent& sessionData)
	{
		std::vector<std::pair<int, int>> availables;
		availables.reserve(sessionData.m_BoardRows * sessionData.m_BoardColumns);
		for (int row = 0; row < sessionData.m_BoardRows; row++)
		{
			for (int col = 0; col < sessionData.m_BoardColumns; col++)
			{
				availables.push_back({ row, col });
			}
		}
		auto view = registry.view<const ttfe::board::TileComponent>();
		for (auto entity : view)
		{
			const auto& tile = view.get<const ttfe::board::TileComponent>(entity);
			auto it = std::find(availables.begin(), availables.end(), std::pair<int, int>{ tile.m_Tile.m_Row, tile.m_Tile.m_Col });
			if (it != availables.end())
				availables.erase(it);
		}
		return availables;
	}

	ttfe::board::Tile GetRandomTile(const ttfe::session::DataComponent& sessionData, std::vector<std::pair<int, int>>& availablePositions)
	{
		assert(!availablePositions.empty());
		ttfe::board::Tile tile;

		std::uniform_int_distribution<std::size_t> numberDist{ 0, sessionData.m_TileNumberBag.size() - 1 };
		std::uniform_int_distribution<std::size_t> positionDist{ 0, availablePositions.size() - 1};

		tile.m_Number = sessionData.m_TileNumberBag[numberDist(s_RandomGen)];

		size_t posIndex = positionDist(s_RandomGen);
		auto& position = availablePositions[posIndex];
		tile.m_Row = position.first;
		tile.m_Col = position.second;
		availablePositions.erase(availablePositions.begin() + posIndex);

		return tile;
	}
}

void ttfe::board::SpawnNewTilesSystem(entt::registry& registry, float)
{
	auto sessionView = registry.view<const ttfe::session::DataComponent>();
	if (entt::is_empty(sessionView))
		return;

	const auto& sessionData = sessionView.get<const ttfe::session::DataComponent>(sessionView.front());
	int tilesToPlace = 0;

	if (registry.all_of<ttfe::session::IsActiveComponent::Added>(sessionView.front()))
	{
		tilesToPlace += sessionData.m_InitialTilesCount;
	}

	auto newTurnView = registry.view<ttfe::session::NewTurnEvent>();
	tilesToPlace += static_cast<int>(newTurnView.size());

	if (tilesToPlace == 0)
		return;

	auto availablePositions = FindAvailableBoardPositions(registry, sessionData);

	for (int i = 0; i < tilesToPlace; i++)
	{
		auto& event = entt::add_event<ttfe::board::CreateTileRequestEvent>(registry);
		event.m_Tile = GetRandomTile(sessionData, availablePositions);
	}
}