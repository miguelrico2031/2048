#include "core/input/InputEventSystem.h"

#include "core/helpers/EnttHelpers.h"
#include "core/input/InputComponents.h"
#include "core/input/InputDefinitions.h"
#include <entt/entt.hpp>
#include <raylib.h>

void core::input::InputEventSystem(entt::registry& registry, float)
{
	entt::clear_events<core::input::KeyPressedEvent>(registry);
	for (int i = 0; i < core::input::c_MaxKeysPressed; i++)
	{
		int key = GetKeyPressed();
		if (key == 0)
			break;
		auto& keyPressed = entt::add_event<core::input::KeyPressedEvent>(registry);
		keyPressed.m_KeyPressed = key;
	}
}
