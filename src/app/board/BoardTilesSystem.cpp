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

	void CreateNewTile(entt::registry& registry, const ttfe::session::DataComponent& sessionData, const ttfe::board::Tile& tile)
	{
		entt::entity entity = registry.create();

		auto& tileComponent = entt::add_component<ttfe::board::TileComponent>(registry, entity);
		tileComponent.m_Tile = tile;

		auto& positionComponent = entt::add_component<core::transform::PositionComponent>(registry, entity);
		positionComponent.m_Position = ttfe::board::BoardToWorld(tileComponent.m_Tile.m_Coords, sessionData);

		auto& spriteComponent = entt::add_component<core::gfx::SpriteComponent>(registry, entity);
		spriteComponent.m_Texture = c_TexHandle;
		spriteComponent.m_Color = LIME;

		auto& textComponent = entt::add_component<core::gfx::TextComponent>(registry, entity);
		textComponent.m_Text = std::to_string(tileComponent.m_Tile.m_Number);

		auto& renderComponent = entt::add_component<core::gfx::RenderComponent>(registry, entity);
		renderComponent.m_RenderOrder = 2;
	}

	void CreateNewTiles(entt::registry& registry, const ttfe::session::DataComponent& sessionData)
	{
		auto newEventView = registry.view<const ttfe::board::CreateNewTileRequestEvent>();
		auto mergeEventView = registry.view<const ttfe::board::CreateMergedTileRequestEvent>();
		if (newEventView.empty() && mergeEventView.empty())
			return;

		for (auto eventEntity : newEventView)
		{
			const auto& event = newEventView.get<const ttfe::board::CreateNewTileRequestEvent>(eventEntity);
			CreateNewTile(registry, sessionData, event.m_Tile);
		}
		for (auto eventEntity : mergeEventView)
		{
			const auto& event = mergeEventView.get<const ttfe::board::CreateMergedTileRequestEvent>(eventEntity);
			CreateNewTile(registry, sessionData, event.m_Tile);
		}
	}

	void UpdateTilesCoords(entt::registry& registry)
	{
		auto view = registry.view<ttfe::board::TileComponent, const ttfe::board::UpdateTileCoordsRequestComponent>();
		for (auto entity : view)
		{
			auto [tileComponent, request] = view.get(entity);
			tileComponent.m_Tile.m_Coords = request.m_Coords;
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
	auto sessionView = registry.view<const ttfe::session::IsActiveComponent, const ttfe::session::DataComponent>();
	if (entt::is_empty(sessionView))
		return;
	const auto& sessionData = sessionView.get<ttfe::session::DataComponent>(sessionView.front());
	CreateNewTiles(registry, sessionData);
	UpdateTilesCoords(registry);
}