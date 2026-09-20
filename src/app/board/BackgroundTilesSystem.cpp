#include "app/board/BackgroundTilesSystem.h"

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
	constexpr ttfe::textures::ID c_TexID = ttfe::textures::ID::Board_BackgroundTile;
	constexpr core::gfx::TextureHandle c_TexHandle = ttfe::textures::ToHandle(c_TexID);
	const char* c_TexPath = ttfe::textures::GetTexturePath(c_TexID);
}

void ttfe::board::BackgroundTilesInitializer(entt::registry& registry)
{
	auto& texturesSingleton = entt::get_singleton<core::gfx::TexturesSingletonComponent>(registry);
	texturesSingleton.m_LoadedTextures[c_TexHandle] = LoadTexture(c_TexPath);
}

void ttfe::board::BackgroundTilesSystem(entt::registry& registry, float)
{
	auto view = registry.view<const ttfe::session::IsActiveComponent::Added, const ttfe::session::DataComponent>();
	if (entt::is_empty(view))
		return;
	
	const auto& dataSingleton = entt::get_singleton<core::app::DataSingletonComponent>(registry);
	Vector2 center{ dataSingleton.m_Data.m_WindowWidth / 2.f, dataSingleton.m_Data.m_WindowHeight / 2.f };
	
	const auto& sessionData = view.get<const ttfe::session::DataComponent>(view.front());
	const int numRows = sessionData.m_BoardRows;
	const int numCols = sessionData.m_BoardColumns;
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			entt::entity tileEntity = registry.create();
			entt::add_component<ttfe::board::BackgroundTileComponent>(registry, tileEntity);
			auto& sprite = entt::add_component<core::gfx::SpriteComponent>(registry, tileEntity);
			sprite.m_Texture = c_TexHandle;

			auto& position = entt::add_component<core::transform::PositionComponent>(registry, tileEntity);
			position.m_Position = ttfe::board::BoardToWorld(row, col, sessionData);
		}
	}
	
}
