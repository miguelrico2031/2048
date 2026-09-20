#include "app/board/BoardTilesSystem.h"

#include "app/board/BoardComponents.h"
#include "app/board/BoardHelpers.h"
#include "app/session/SessionComponents.h"
#include "app/textures/TexturesDefinitions.h"
#include "core/app/AppComponents.h"
#include "core/gfx/GfxComponents.h"
#include "core/helpers/EnttHelpers.h"
#include "core/transform/TransformComponents.h"
#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>

namespace
{
	constexpr ttfe::textures::ID c_TexID = ttfe::textures::ID::Board_Tile;
	constexpr core::gfx::TextureHandle c_TexHandle = ttfe::textures::ToHandle(c_TexID);
	const char* c_TexPath = ttfe::textures::GetTexturePath(c_TexID);

	void CreateNewTiles(entt::registry& registry)
	{
		auto sessionView = registry.view<const ttfe::session::IsActiveComponent, const ttfe::session::DataComponent>();
		if (entt::is_empty(sessionView))
			return;

		auto eventView = registry.view<const ttfe::board::CreateTileRequestEvent>();
		if (eventView.empty())
			return;

		const auto& sessionData = sessionView.get<ttfe::session::DataComponent>(sessionView.front());

		for (auto eventEntity : eventView)
		{
			const auto& event = eventView.get<const ttfe::board::CreateTileRequestEvent>(eventEntity);

			entt::entity entity = registry.create();

			auto& tileComponent = entt::add_component<ttfe::board::TileComponent>(registry, entity);
			tileComponent.m_Tile = event.m_Tile;

			auto& positionComponent = entt::add_component<core::transform::PositionComponent>(registry, entity);
			positionComponent.m_Position = ttfe::board::BoardToWorld(tileComponent.m_Tile.m_Row, tileComponent.m_Tile.m_Col, sessionData);

			auto& spriteComponent = entt::add_component<core::gfx::SpriteComponent>(registry, entity);
			spriteComponent.m_Texture = c_TexHandle;
			spriteComponent.m_Color = LIME;

			auto& textComponent = entt::add_component<core::gfx::TextComponent>(registry, entity);
			textComponent.m_Text = std::to_string(tileComponent.m_Tile.m_Number);

			auto& renderComponent = entt::add_component<core::gfx::RenderComponent>(registry, entity);
			renderComponent.m_RenderOrder = 2.f;
		}
	}
}

void ttfe::board::BoardTilesInitializer(entt::registry& registry)
{
	auto& texturesSingleton = entt::get_singleton<core::gfx::TexturesSingletonComponent>(registry);
	texturesSingleton.m_LoadedTextures[c_TexHandle] = LoadTexture(c_TexPath);
}

void ttfe::board::BoardTilesSystem(entt::registry& registry, float)
{
	CreateNewTiles(registry);
}