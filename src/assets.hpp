#pragma once

#include "primitives.hpp"
#include "memory.hpp"
#include "shell_config.hpp"

enum AssetType {
    texture,
    sfx,
    music,
    shader,
    font,
    level,
};

struct AssetData {
    size_t size;
    u8* data;
};

enum TextureFormat {
    rgb,
    rgba,
};

struct TextureHeader {
    u32 width;
    u32 height;
    TextureFormat format;
};

Result<AssetData> asset_load_data(
    ShellConfig const& config,
    RegionMemoryBuffer* dest_memory,
    const AssetType asset_type,
    const char* asset_name
);
