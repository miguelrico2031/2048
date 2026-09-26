#include "app/session/StartSessionSystem.h"

#include "app/session/SessionComponents.h"
#include "app/session/SessionHelpers.h"
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
		dataComponent = ttfe::session::GetSessionDataFromConfig();
		dataComponent.m_BoardCenter = { dataSingleton.m_Data.m_WindowWidth / 2.f, dataSingleton.m_Data.m_WindowHeight / 2.f };
		
		entt::entity turnEntity = registry.create();
		auto& spawningTile = entt::add_component<ttfe::turn::SpawningTileComponent>(registry, turnEntity);
		spawningTile.m_Tiles = 2;
	}
}
