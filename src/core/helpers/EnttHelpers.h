#pragma once
#include <cassert>
#include <entt/entt.hpp>
#include <type_traits>

namespace entt
{
	template<typename T>
	concept CObservable = requires
	{
		typename T::Added;
	};

	template<typename T>
	concept CEmpty = std::is_empty_v<T>;

	template<typename T>
	concept CNonEmpty = !std::is_empty_v<T>;

	template<typename T>
	concept CObservableEmpty = CObservable<T> && CEmpty<T>;

	template<typename T>
	concept CObservableNonEmpty = CObservable<T> && !CEmpty<T>;

	template<typename T>
	concept CNonObservableEmpty = !CObservable<T> && CEmpty<T>;

	template<typename T>
	concept CNonObservableNonEmpty = !CObservable<T> && !CEmpty<T>;

	template<CNonEmpty T>
	T& get_singleton(entt::registry& registry)
	{
		auto view = registry.view<T>();
		entt::entity entity = view.empty()
			? registry.create()
			: view.front();
		return registry.get_or_emplace<T>(entity);
	}

	template<typename T>
	bool check_event(entt::registry& registry)
	{
		auto view = registry.view<T>();
		return !view.empty();
	}

	template<CNonEmpty T>
	T& add_event(entt::registry& registry)
	{
		entt::entity entity = registry.create();
		registry.emplace<T>(entity);
		return registry.get<T>(entity);
	}

	template<CEmpty T>
	void add_event(entt::registry& registry)
	{
		assert(registry.view<T>().empty());
		entt::entity entity = registry.create();
		registry.emplace<T>(entity);
	}


	template<typename T>
	void clear_events(entt::registry& registry)
	{
		auto view = registry.view<T>();
		for(auto entity : view)
			registry.destroy(entity);
	}

	template<CObservableNonEmpty T>
	T& add_component(entt::registry& registry, entt::entity entity)
	{
		registry.emplace<typename T::Added>(entity);
		return registry.emplace<T>(entity);
	}

	template<CObservableEmpty T>
	void add_component(entt::registry& registry, entt::entity entity)
	{
		registry.emplace<typename T::Added>(entity);
		registry.emplace<T>(entity);
	}

	template<CNonObservableNonEmpty T>
	T& add_component(entt::registry& registry, entt::entity entity)
	{
		return registry.emplace<T>(entity);
	}

	template<CNonObservableEmpty T>
	void add_component(entt::registry& registry, entt::entity entity)
	{
		registry.emplace<T>(entity);
	}

	template<CObservable T>
	void clear_observables(entt::registry& registry)
	{
		auto view = registry.view<T, typename T::Added>();
		for (auto entity : view)
			registry.remove<typename T::Added>(entity);
	}

	template<typename T>
	bool is_empty(T& view)
	{
		return view.begin() == view.end();
	}
}