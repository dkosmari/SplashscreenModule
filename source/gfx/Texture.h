#pragma once

#include <cstdint>
#include <gx2/texture.h>
#include <memory>

class Texture {
public:
    // allow moving
    Texture(Texture &&other) noexcept            = default;
    Texture &operator=(Texture &&other) noexcept = default;

    Texture() noexcept = default;

    Texture(std::uint32_t width, std::uint32_t height);

    ~Texture() noexcept;

    GX2Texture *get() noexcept;

    void flush() noexcept;

    void *getPixels() noexcept;

    std::size_t getSize() const noexcept;

    // size of a row, in pixels
    std::uint32_t getRowPitch() noexcept;

    // size of a row, in bytes
    std::uint32_t getRowStride() noexcept;

    uint32_t *getRow(uint32_t y) noexcept;

    explicit operator bool() const noexcept;

private:
    std::unique_ptr<GX2Texture> mTexture;

}; // class Texture
