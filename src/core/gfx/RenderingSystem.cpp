#include "core/gfx/RenderingSystem.h"

#include "core/helpers/EnttHelpers.h"
#include "core/gfx/GfxComponents.h"
#include "core/transform/TransformComponents.h"
#include <entt/entt.hpp>
#include <raylib.h>
#include <unordered_map>
#include <vector>

namespace
{
	void RenderSprite(
		const core::gfx::TexturesSingletonComponent& texturesSingleton,
		const core::gfx::SpriteComponent& sprite,
		const core::transform::PositionComponent& position,
		const core::transform::RotationComponent* rotation,
		const core::transform::ScaleComponent* scale
		)
	{
		const Texture2D& texture = texturesSingleton.m_LoadedTextures.at(sprite.m_Texture);
		const Vector2 spriteSize = Vector2Multiply({ static_cast<float>(texture.width), static_cast<float>(texture.height) },
			scale ? scale->m_Scale : Vector2One());	
		const Vector2 origin = Vector2Scale(spriteSize, 0.5f);
		const Rectangle source{
			0.0f,
			0.0f,
			static_cast<float>(texture.width),
			static_cast<float>(texture.height)
		};
		const Rectangle destination{
			position.m_Position.x,
			position.m_Position.y,
			spriteSize.x,
			spriteSize.y
		};

		DrawTexturePro(
			texture,
			source,
			destination,
			origin,
			rotation ? rotation->m_Rotation : 0.f,
			sprite.m_Color
		);
	}

	void RenderText(
		const core::gfx::FontsSingletonComponent& fontsSingleton,
		const core::gfx::TextComponent& text,
		const core::transform::PositionComponent& position,
		const core::transform::RotationComponent* rotation
		)
	{
		const Font& font = fontsSingleton.m_LoadedFonts.at(text.m_Font);
		const char* textStr = text.m_Text.c_str();
		const Vector2 size = MeasureTextEx(font, textStr, text.m_Size, core::gfx::c_DefaultFontSpacing);
		const Vector2 origin{ size.x / 2.0f, size.y / 2.0f };

		DrawTextPro(
			font,
			textStr,
			position.m_Position,
			origin,
			rotation ? rotation->m_Rotation : 0.f,
			text.m_Size,
			core::gfx::c_DefaultFontSpacing,
			text.m_Color
		);	
	}
}

void core::gfx::RenderingSystem(entt::registry& registry, float)
{	
	auto newSpriteView = registry.view<core::gfx::SpriteComponent::Added>();
	for (auto entity : newSpriteView)
	{
		registry.get_or_emplace<core::gfx::RenderComponent>(entity);
	}

	auto newTextView = registry.view<core::gfx::TextComponent::Added>();
	for (auto entity : newTextView)
	{
		registry.get_or_emplace<core::gfx::RenderComponent>(entity);
	}

	if (!newSpriteView.empty() || !newTextView.empty())
	{
		registry.sort<core::gfx::RenderComponent>(
			[&](entt::entity lhs, entt::entity rhs)
			{
				const auto& lhsOrder = registry.get<core::gfx::RenderComponent>(lhs);
				const auto& rhsOrder = registry.get<core::gfx::RenderComponent>(rhs);

				return lhsOrder.m_RenderOrder < rhsOrder.m_RenderOrder;
			}
		);
	}

	const auto& fontsSingleton = entt::get_singleton<core::gfx::FontsSingletonComponent>(registry);
	const auto& texturesSingleton = entt::get_singleton<core::gfx::TexturesSingletonComponent>(registry);

	auto renderView = registry.view<const core::gfx::RenderComponent, const core::transform::PositionComponent>();
	for (auto entity : renderView)
	{
		auto [renderComponent, positionComponent] = renderView.get(entity);

		const auto* rotationComponent = registry.try_get<core::transform::RotationComponent>(entity);

		if (const auto* spriteComponent = registry.try_get<core::gfx::SpriteComponent>(entity))
		{
			const auto* scaleComponent = registry.try_get<core::transform::ScaleComponent>(entity);
			RenderSprite(texturesSingleton, *spriteComponent, positionComponent, rotationComponent, scaleComponent);
		}
		if (const auto* textComponent = registry.try_get<core::gfx::TextComponent>(entity))
		{
			RenderText(fontsSingleton, *textComponent, positionComponent, rotationComponent);
		}
		
	}

	entt::clear_observables<core::gfx::SpriteComponent>(registry);
	entt::clear_observables<core::gfx::TextComponent>(registry);
}
