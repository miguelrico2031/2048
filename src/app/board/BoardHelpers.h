#pragma once

#include <entt/entity/fwd.hpp>
#include <raylib.h>

namespace ttfe::session
{
	struct DataComponent;
}
namespace ttfe::board
{
	Vector2 BoardToWorld(int row, int col, const ttfe::session::DataComponent& sessionData);
}