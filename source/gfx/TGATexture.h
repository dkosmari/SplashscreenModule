#pragma once

#include "Texture.h"
#include <cstddef>
#include <expected>
#include <span>
#include <string>

// quick and dirty 24-bit TGA loader
std::expected<Texture, std::string> TGA_LoadTexture(std::span<const std::byte> data) noexcept;
