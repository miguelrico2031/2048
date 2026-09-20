#pragma once
#include <entt/entt.hpp>

namespace entt
{
	template<typename T>
	T& get_singleton(entt::registry& registry, entt::entity& out_Entity)
	{
		auto view = registry.view<T>();
		if (!view.empty())
		{
			out_Entity = view.front();
		}
		else
		{
			out_Entity = registry.create();
			registry.emplace<T>(out_Entity);
		}
		return registry.get<T>(out_Entity);
	}

	template<typename T>
	T& get_singleton(entt::registry& registry)
	{
		entt::entity entity;
		return get_singleton<T>(registry, entity);
	}

	template<typename T>
	bool is_empty(T& view)
	{
		return view.begin() == view.end();
	}
}