#pragma once

#include "core/app/AppComponents.h"
#include <entt/entt.hpp>
#include <functional>
#include <vector>

namespace core
{
	struct App
	{
		core::app::Data m_Data;
		entt::registry m_Registry;
		std::vector<std::function<void(entt::registry&, float)>> m_Systems;
		std::vector<std::function<void(entt::registry&)>> m_Initializers;
	};

	void Run(core::App& app);
}