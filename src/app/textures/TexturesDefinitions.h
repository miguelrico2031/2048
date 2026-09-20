#pragma once

#include "core/gfx/GfxDefinitions.h"
#include <array>
#include <cassert>

namespace ttfe::textures
{
	enum class ID : uint8_t
	{
		None = 0,
		Board_BackgroundTile,
		Board_Tile,

        MAX
	};

    constexpr std::array<const char*, static_cast<size_t>(ID::MAX)> c_TexturePaths{
       nullptr,
       "assets/background_tile.png",
       "assets/board_tile.png"
    };

    constexpr const char* GetTexturePath(ID id)
    {
        assert(id != ID::MAX);
        return c_TexturePaths[static_cast<size_t>(id)];
    }

    constexpr core::gfx::TextureHandle ToHandle(ID id)
    {
        return static_cast<core::gfx::TextureHandle>(id);
    }
}