#include "core/sprite/SpriteRenderingSystem.h"

#include "core/helpers/EnttHelpers.h"
#include "core/sprite/SpriteComponents.h"
#include "core/transform/TransformComponents.h"
#include <entt/entt.hpp>
#include <raylib.h>
#include <unordered_map>
#include <vector>

namespace
{
	struct SpriteRenderData
	{
		Vector2 m_Position = {0.f, 0.f};
		float m_Rotation = 0.f;
		Color m_Color = WHITE;
	};
}

void core::sprite::SpriteRenderingSystem(entt::registry& registry, float)
{
	auto view = registry.view<const core::sprite::SpriteComponent, const core::transform::PositionComponent>();
	if (entt::is_empty(view))
		return;

	const auto& texturesSingleton = entt::get_singleton<core::sprite::TexturesSingletonComponent>(registry);

	std::unordered_map<core::sprite::TextureHandle, std::vector<SpriteRenderData>> renderTexturesMap{};

	for (entt::entity entity : view)
	{
		auto[sprite, position] = view.get(entity);

		SpriteRenderData& spriteData = renderTexturesMap[sprite.m_Texture].emplace_back();
		spriteData.m_Position = position.m_Position;
		spriteData.m_Color = sprite.m_Color;
		if (const auto* rotationComponent = registry.try_get<const core::transform::RotationComponent>(entity))
		{
			spriteData.m_Rotation = rotationComponent->m_Rotation;
		}
	}

	for (auto&& [textureHandle, spriteDataArray] : renderTexturesMap)
	{
		const Texture2D& texture = texturesSingleton.m_LoadedTextures.at(textureHandle);
		
		const Vector2 origin{texture.width / 2.0f, texture.height / 2.0f };
		const Rectangle source{
			0.0f,
			0.0f,
			static_cast<float>(texture.width),
			static_cast<float>(texture.height)
		};

		Rectangle destination{
			0.f,
			0.f,
			static_cast<float>(texture.width),
			static_cast<float>(texture.height)
		};

		for (const SpriteRenderData& spriteData : spriteDataArray)
		{
			destination.x = spriteData.m_Position.x;
			destination.y = spriteData.m_Position.y;
			DrawTexturePro(
				texture,
				source,
				destination,
				origin,
				spriteData.m_Rotation,
				spriteData.m_Color
			);
		}
	
	}
}
