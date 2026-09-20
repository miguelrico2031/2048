#include "core/input/InputEventSystem.h"

#include "core/helpers/EnttHelpers.h"
#include "core/input/InputComponents.h"
#include <entt/entt.hpp>
#include <raylib.h>
#include <array>


void core::input::InputEventSystem(entt::registry& registry, float)
{
	entt::entity inputEntity;
	entt::get_singleton<core::input::InputSingletonComponent>(registry, inputEntity);

	std::array<int, core::input::c_MaxKeysPressed> keysPressed{};
	int numKeys = 0;
	while (int keyPressed = GetKeyPressed())
	{
		keysPressed[numKeys++] = keyPressed;
		if (numKeys >= core::input::c_MaxKeysPressed)
			break;
	}
	if (keysPressed.empty())
	{
		registry.remove<core::input::KeyPressedComponent>(inputEntity);
	}
	else
	{
		auto* keyPressed = registry.try_get<core::input::KeyPressedComponent>(inputEntity);
		if (!keyPressed)
		{
			keyPressed = &registry.emplace<core::input::KeyPressedComponent>(inputEntity);
		}
		for (int i = 0; i < core::input::c_MaxKeysPressed; i++)
		{
			keyPressed->m_KeysPressed[i] = i < numKeys
				? keysPressed[i]
				: 0;
		}
	}
}
