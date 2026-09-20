#include "Test.h"

#include "core/helpers/EnttHelpers.h"
#include "core/sprite/SpriteComponents.h"
#include "core/transform/TransformComponents.h"
#include <iostream>
#include <entt/entt.hpp>

void TestInit(entt::registry& registry)
{
	auto& texturesSingleton = entt::get_singleton<core::sprite::TexturesSingletonComponent>(registry);

	texturesSingleton.m_LoadedTextures[20] = LoadTexture("assets/test.png");

	entt::entity entity = registry.create();
	auto& sprite = registry.emplace<core::sprite::SpriteComponent>(entity);
	sprite.m_Texture = 20;
	registry.emplace<core::transform::PositionComponent>(entity).m_Position = { 1280/2, 720/2 };
}
void TestSystem(entt::registry& registry, float)
{
	static int times = 10;
	if(times-- > 0)
		std::cout << "Hi\n";
}