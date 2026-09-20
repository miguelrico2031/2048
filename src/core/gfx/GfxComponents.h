#pragma once

#include "core/gfx/GfxDefinitions.h"

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <entt/entity/fwd.hpp>

namespace core::gfx
{
	struct TexturesSingletonComponent
	{
		std::unordered_map<core::gfx::TextureHandle, Texture2D> m_LoadedTextures;
	};

	struct FontsSingletonComponent
	{
		std::unordered_map<core::gfx::FontHandle, Font> m_LoadedFonts;
	};

	struct SpriteComponent
	{
		core::gfx::TextureHandle m_Texture = -1;
		Color m_Color = WHITE;
		struct Added {};
	};

	struct TextComponent
	{
		std::string m_Text;
		core::gfx::FontHandle m_Font = 0;
		Color m_Color = WHITE;
		float m_Size = 20.f;
		struct Added {};
	};

	struct RenderComponent
	{
		int m_RenderOrder = 1;
	};
}