#include "Test.h"

#include "core/input/InputComponents.h"
#include "core/helpers/EnttHelpers.h"
#include "core/gfx/GfxComponents.h"
#include "core/transform/TransformComponents.h"
#include <entt/entt.hpp>
#include <iostream>

namespace
{
	entt::entity testEntity;
}

void TestInit(entt::registry& registry)
{
	//auto& texturesSingleton = entt::get_singleton<core::gfx::TexturesSingletonComponent>(registry);
	//texturesSingleton.m_LoadedTextures[20] = LoadTexture("assets/test.png");

	testEntity = registry.create();
	//auto& sprite = entt::add_component<core::gfx::SpriteComponent>(registry, testEntity);
	//sprite.m_Texture = 20;
	//auto& text = entt::add_component<core::gfx::TextComponent>(registry, testEntity);
	//text.m_Text = "Candelovich, es una\nsonofabich....\nmuylinda";
	//text.m_Color = BLACK;
	//text.m_Size = 40.f;
	//text.m_Font = 0;
	auto& position = entt::add_component<core::transform::PositionComponent>(registry, testEntity);
	position.m_Position = { 1280/2, 720/2 };
}
void TestSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<core::input::KeyPressedEvent>();
	for (auto entity : view)
	{
		int key = view.get<core::input::KeyPressedEvent>(entity).m_KeyPressed;
		
		Vector2 movement{ 0.f, 0.f };
		if (key == KEY_LEFT)
		{
			movement.x -= 1.f;
		}
		else if (key == KEY_RIGHT)
		{
			movement.x += 1.f;
		}
		else if (key == KEY_UP)
		{
			movement.y -= 1.f;
		}
		else if (key == KEY_DOWN)
		{
			movement.y += 1.f;
		}

		if (movement.x != 0.f || movement.y != 0.f)
		{
			auto& transform = registry.get<core::transform::PositionComponent>(testEntity);
			transform.m_Position = Vector2Add(transform.m_Position, Vector2Scale(movement, dt * 1000.f));
		}
	}
	
}