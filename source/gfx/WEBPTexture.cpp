#include "WEBPTexture.h"
#include <stdexcept>
#include <webp/decode.h>

using namespace std::literals;

std::expected<Texture, std::string> WEBP_LoadTexture(std::span<const uint8_t> data) noexcept {
    try {
        int width, height;

        if (!WebPGetInfo(data.data(), data.size(), &width, &height)) {
            throw std::runtime_error{"Failed to parse WEBP header"};
        }

        Texture texture(width, height);

        if (!WebPDecodeRGBAInto(data.data(), data.size(),
                                reinterpret_cast<uint8_t *>(texture.getPixels()),
                                texture.getSize(),
                                texture.getRowStride())) {
            throw std::runtime_error{"Failed to decode WEBP image"};
        }

        texture.flush();
        return texture;
    } catch (std::exception &e) {
        return std::unexpected{"[WEBPTexture] "s + e.what()};
    }
}
