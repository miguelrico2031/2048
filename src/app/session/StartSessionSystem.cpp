#include "app/session/StartSessionSystem.h"

#include "app/session/SessionComponents.h"
#include "app/turn/TurnComponents.h"
#include "core/app/AppComponents.h"
#include "core/helpers/EnttHelpers.h"
#include "core/input/InputComponents.h"
#include <entt/entt.hpp>
#include <raylib.h>

void ttfe::session::StartSessionSystem(entt::registry& registry, float)
{
	auto view = registry.view<ttfe::session::IsActiveComponent>();
	if (!view.empty())
	{
		entt::clear_observables<ttfe::session::IsActiveComponent>(registry);
		return;
	}
	if (entt::check_event<core::input::KeyPressedEvent>(registry))
	{
		const auto& dataSingleton = entt::get_singleton<core::app::DataSingletonComponent>(registry);

		entt::entity sessionEntity = registry.create();
		entt::add_component<ttfe::session::IsActiveComponent>(registry, sessionEntity);
		auto& dataComponent = entt::add_component<ttfe::session::DataComponent>(registry, sessionEntity);
		dataComponent.m_BoardColumns = 4;
		dataComponent.m_BoardRows = 4;
		dataComponent.m_BoardTilesSize = 64;
		dataComponent.m_BoardTilesPadding = 4;
		dataComponent.m_BoardCenter = { dataSingleton.m_Data.m_WindowWidth / 2.f, dataSingleton.m_Data.m_WindowHeight / 2.f };
		dataComponent.m_TileNumberBag = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 4 };
		
		entt::entity turnEntity = registry.create();
		auto& spawningTile = entt::add_component<ttfe::turn::SpawningTileComponent>(registry, turnEntity);
		spawningTile.m_Tiles = 2;
	}
}
