#pragma once

#include "Texture.h"
#include <cstddef>
#include <expected>
#include <span>
#include <string>

std::expected<Texture, std::string> JPEG_LoadTexture(std::span<const std::byte> data) noexcept;
