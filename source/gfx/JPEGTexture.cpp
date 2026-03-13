#include "JPEGTexture.h"
#include <stdexcept>
#include <turbojpeg.h>

using namespace std::literals;

struct TJError : std::runtime_error {
    TJError(tjhandle handle) : std::runtime_error{tj3GetErrorStr(handle)} {}
};

// RAII wrapper for tjhandle
class JPEGImage {
public:
    JPEGImage() : mHandle{tj3Init(TJINIT_DECOMPRESS)} {
        if (!mHandle) {
            throw TJError{mHandle};
        }
    }

    ~JPEGImage() noexcept {
        if (mHandle)
            tj3Destroy(mHandle);
    }

    void decompressHeader(std::span<const uint8_t> data) {
        if (tj3DecompressHeader(mHandle, data.data(), data.size())) {
            throw TJError{mHandle};
        }
    }

    void decompress8(std::span<const uint8_t> data, void *dst,
                     std::uint32_t rowStride, int pixelFormat) {
        if (tj3Decompress8(mHandle,
                           data.data(), data.size(),
                           static_cast<unsigned char *>(dst),
                           rowStride,
                           pixelFormat)) {
            throw TJError{mHandle};
        }
    }

    std::uint32_t getWidth() const {
        int width = tj3Get(mHandle, TJPARAM_JPEGWIDTH);
        if (width < 0) {
            throw std::runtime_error{"Unknown JPEG width"};
        }
        return width;
    }

    std::uint32_t getHeight() const {
        int height = tj3Get(mHandle, TJPARAM_JPEGHEIGHT);
        if (height < 0) {
            throw std::runtime_error{"Unknown JPEG height"};
        }
        return height;
    }

private:
    tjhandle mHandle = nullptr;
}; // class JPEGImage

std::expected<Texture, std::string> JPEG_LoadTexture(std::span<const uint8_t> data) noexcept {
    try {
        JPEGImage jpeg;
        jpeg.decompressHeader(data);
        Texture texture{jpeg.getWidth(), jpeg.getHeight()};
        jpeg.decompress8(data, texture.getPixels(), texture.getRowStride(), TJPF_RGBA);
        texture.flush();
        return texture;
    } catch (std::exception &e) {
        return std::unexpected{"[JPEGTexture] "s + e.what()};
    }
}
