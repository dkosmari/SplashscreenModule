#pragma once

#include "Texture.h"
#include <cstdint>
#include <expected>
#include <span>
#include <string>

std::expected<Texture, std::string> JPEG_LoadTexture(std::span<const uint8_t> data) noexcept;
