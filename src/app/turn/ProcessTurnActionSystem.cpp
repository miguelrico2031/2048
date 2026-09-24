#include "app/turn/ProcessTurnActionSystem.h"

#include "app/movement/MovementComponents.h"
#include "app/turn/TurnComponents.h"
#include "core/helpers/EnttHelpers.h"
#include "core/input/InputComponents.h"
#include <entt/entt.hpp>
#include <raylib.h>

void ttfe::turn::ProcessTurnActionSystem(entt::registry& registry, float)
{
	entt::clear_events<ttfe::move::BoardMovementRequestEvent>(registry);

	const auto inputView = registry.view<const core::input::KeyPressedEvent>();
	if (inputView.size() != 1) // Multiple keys pressed at once discards the input for a turn action
		return;

	const auto canActView = registry.view<const ttfe::turn::ExpectingActionComponent>();
	if (canActView.empty())
		return;

	const int key = inputView.get<core::input::KeyPressedEvent>(inputView.front()).m_KeyPressed;
	ttfe::move::Direction direction;
	switch (key)
	{
	case KEY_LEFT:
	case KEY_A:
		direction = ttfe::move::Direction::Left;
		break;
	case KEY_RIGHT:
	case KEY_D:
		direction = ttfe::move::Direction::Right;
		break;
	case KEY_DOWN:
	case KEY_S:
		direction = ttfe::move::Direction::Down;
		break;
	case KEY_UP:
	case KEY_W:
		direction = ttfe::move::Direction::Up;
		break;
	default:
		return;
	}

	auto& event = entt::add_event<ttfe::move::BoardMovementRequestEvent>(registry);
	event.m_Direction = direction;
}
