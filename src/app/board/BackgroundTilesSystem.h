#pragma once
#include <entt/entity/fwd.hpp>

namespace ttfe::board
{
	void BackgroundTilesInitializer(entt::registry& registry);
	void BackgroundTilesSystem(entt::registry& registry, float);
}