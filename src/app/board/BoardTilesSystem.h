#pragma once
#include <entt/entity/fwd.hpp>

namespace ttfe::board
{
	void BoardTilesInitializer(entt::registry& registry);
	void BoardTilesSystem(entt::registry& registry, float);
}