#include "PNGTexture.h"
#include <png.h>
#include <stdexcept>

using namespace std::literals;

// RAII wrapper for png_image
class PNGImage {
public:
    PNGImage() {
        mImage.version = PNG_IMAGE_VERSION;
    }

    ~PNGImage() noexcept {
        png_image_free(&mImage);
    }

    void beginRead(std::span<const uint8_t> data) {
        if (!png_image_begin_read_from_memory(&mImage, data.data(), data.size())) {
            throw std::runtime_error{"Failed to parse PNG header: "s + mImage.message};
        }
    }

    void finishRead(png_const_colorp background, void *dst,
                    png_int_32 rowStride, void *colormap) {
        // Request the output to always be RGBA
        mImage.format = PNG_FORMAT_RGBA;
        if (!png_image_finish_read(&mImage, background, dst, rowStride, nullptr)) {
            throw std::runtime_error{"Failed to read PNG image: "s + mImage.message};
        }
    }

    std::uint32_t
    getWidth() const noexcept {
        return mImage.width;
    }

    std::uint32_t getHeight() const noexcept {
        return mImage.height;
    }

private:
    png_image mImage{};

}; // class PNGImage

std::expected<Texture, std::string> PNG_LoadTexture(std::span<const uint8_t> data) noexcept {
    try {
        PNGImage png;
        png.beginRead(data);
        Texture texture{png.getWidth(), png.getHeight()};
        png.finishRead(nullptr, texture.getPixels(), texture.getRowStride(), nullptr);
        texture.flush();
        return texture;
    } catch (std::exception &e) {
        return std::unexpected{"[PNGTexture] "s + e.what()};
    }
}
