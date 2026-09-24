#include "app/movement/MoveTilesSystem.h"

#include "app/board/BoardComponents.h"
#include "app/board/BoardHelpers.h"
#include "app/movement/MovementComponents.h"
#include "app/session/SessionComponents.h"
#include "app/turn/TurnComponents.h"
#include "core/helpers/EnttHelpers.h"
#include "core/transform/TransformComponents.h"
#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>

namespace
{
	void StartMovingTiles(entt::registry& registry)
	{
		constexpr float SPEED = 1000.f;

		const auto tileView = registry.view<const ttfe::move::MoveTileRequestComponent, const core::transform::PositionComponent>();
		if (entt::is_empty(tileView))
			return;

		const auto sessionView = registry.view<const ttfe::session::DataComponent>();
		const auto& sessionData = sessionView.get<const ttfe::session::DataComponent>(sessionView.front());

		for (auto entity : tileView)
		{
			const auto& moveTileRequest = tileView.get<const ttfe::move::MoveTileRequestComponent>(entity);
			const auto& positionComponent = tileView.get<const core::transform::PositionComponent>(entity);
			
			auto& isMovingComponent = entt::add_component<ttfe::move::IsMovingComponent>(registry, entity);
			isMovingComponent.m_TargetPosition = ttfe::board::BoardToWorld(moveTileRequest.m_Target, sessionData);
			Vector2 directionToTarget = Vector2Subtract(isMovingComponent.m_TargetPosition, positionComponent.m_Position);
			isMovingComponent.m_Movement = Vector2Scale(Vector2Normalize(directionToTarget), SPEED);
			isMovingComponent.m_TargetCoords = moveTileRequest.m_Target;
		}
	}

	void UpdateMovingTiles(entt::registry& registry, float dt)
	{
		entt::clear_events<ttfe::board::CreateMergedTileRequestEvent>(registry);
		entt::clear_temp_components<ttfe::board::UpdateTileCoordsRequestComponent>(registry);

		auto tileView = registry.view<const ttfe::move::IsMovingComponent, core::transform::PositionComponent>();
		if (entt::is_empty(tileView))
			return;

		for (auto entity : tileView)
		{
			auto [isMovingComponent, positionComponent] = tileView.get(entity);
			Vector2 frameMovement = Vector2Scale(isMovingComponent.m_Movement, dt);
			positionComponent.m_Position = Vector2Add(positionComponent.m_Position, frameMovement);

			Vector2 directionToTarget = Vector2Subtract(isMovingComponent.m_TargetPosition, positionComponent.m_Position);
			if (Vector2DotProduct(directionToTarget, isMovingComponent.m_Movement) <= 0.0f)
			{
				positionComponent.m_Position = isMovingComponent.m_TargetPosition;
				const ttfe::board::Coords targetCoords = isMovingComponent.m_TargetCoords;
				registry.remove<ttfe::move::IsMovingComponent>(entity);

				if (const auto* mergingComponent = registry.try_get<const ttfe::board::MergingTileComponent>(entity))
				{
					if (!registry.all_of<ttfe::move::IsMovingComponent>(mergingComponent->m_Other))
					{
						const auto& tileComponent = registry.get<const ttfe::board::TileComponent>(entity);
						auto& request = entt::add_event<ttfe::board::CreateMergedTileRequestEvent>(registry);
						request.m_Tile = tileComponent.m_Tile;
						request.m_Tile.m_Coords = targetCoords;
						request.m_Tile.m_Number *= 2;

						registry.destroy(mergingComponent->m_Other);
						registry.destroy(entity);
					}
				}
				else
				{
					auto& request = entt::add_component<ttfe::board::UpdateTileCoordsRequestComponent>(registry, entity);
					request.m_Coords = targetCoords;
				}
			}
		}
	}

	void AdvanceTurn(entt::registry& registry)
	{
		auto isMovingView = registry.view<const ttfe::move::IsMovingComponent>();
		if (!isMovingView.empty())
			return;

		auto turnView = registry.view<ttfe::turn::MovingTilesComponent>();
		if (turnView.empty())
			return;

		auto turnEntity = turnView.front();
		registry.remove<ttfe::turn::MovingTilesComponent>(turnEntity);
		entt::add_component<ttfe::turn::SpawningTileComponent>(registry, turnEntity);
	}
}

void ttfe::move::MoveTilesSystem(entt::registry& registry, float dt)
{
	StartMovingTiles(registry);
	UpdateMovingTiles(registry, dt);
	AdvanceTurn(registry);
}