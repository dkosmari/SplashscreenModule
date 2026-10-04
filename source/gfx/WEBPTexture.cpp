#include "WEBPTexture.h"
#include "utils/utils.h"
#include <stdexcept>
#include <webp/decode.h>

using namespace std::literals;

std::expected<Texture, std::string> WEBP_LoadTexture(std::span<const std::byte> data) noexcept {
    try {
        int width, height;

        auto udata = ToSpan<uint8_t>(data);
        if (!WebPGetInfo(udata.data(),
                         udata.size(),
                         &width,
                         &height)) {
            throw std::runtime_error{"Failed to parse WEBP header"};
        }

        Texture texture(width, height);

        if (!WebPDecodeRGBAInto(udata.data(), udata.size(),
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
