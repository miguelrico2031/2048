#pragma once
#include "core/input/InputDefinitions.h"

namespace core::input
{
	struct InputSingletonComponent { bool _; }; //Dummy member is necessary to avoid entt empty component optimization

	struct KeyPressedComponent
	{
		int m_KeysPressed[core::input::c_MaxKeysPressed];
	};
}