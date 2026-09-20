#pragma once
#include <entt/entt.hpp>
#include <functional>
#include <vector>
#include <string>

namespace core
{
	struct App
	{
		std::string m_Name = "App";
		int m_WindowWidth = 640;
		int m_WindowHeight = 360;
		entt::registry m_Registry;
		std::vector<std::function<void(entt::registry&, float)>> m_Systems;
		std::vector<std::function<void(entt::registry&)>> m_Initializers;
	};

	void Run(core::App& app);
}