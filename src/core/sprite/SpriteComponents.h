#pragma once

#include "core/sprite/SpriteDefinitions.h"

#include <raylib.h>
#include <unordered_map>

namespace core::sprite
{
	struct TexturesSingletonComponent
	{
		std::unordered_map<core::sprite::TextureHandle, Texture2D> m_LoadedTextures;
	};

	struct SpriteComponent
	{
		core::sprite::TextureHandle m_Texture = -1;
		Color m_Color = WHITE;
	};
}