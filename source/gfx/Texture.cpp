#include "Texture.h"
#include <cstdlib>
#include <cstring>
#include <gx2/mem.h>
#include <stdexcept>

Texture::Texture(std::uint32_t width, std::uint32_t height) : mTexture{std::make_unique<GX2Texture>()} {
    std::memset(mTexture.get(), 0, sizeof *mTexture);
    mTexture->surface.width     = width;
    mTexture->surface.height    = height;
    mTexture->surface.depth     = 1;
    mTexture->surface.mipLevels = 1;
    mTexture->surface.format    = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
    mTexture->surface.aa        = GX2_AA_MODE1X;
    mTexture->surface.use       = GX2_SURFACE_USE_TEXTURE;
    mTexture->surface.dim       = GX2_SURFACE_DIM_TEXTURE_2D;
    mTexture->surface.tileMode  = GX2_TILE_MODE_LINEAR_ALIGNED;
    mTexture->surface.swizzle   = 0;
    mTexture->viewFirstMip      = 0;
    mTexture->viewNumMips       = 1;
    mTexture->viewFirstSlice    = 0;
    mTexture->viewNumSlices     = 1;
    mTexture->compMap           = 0x0010203;
    GX2CalcSurfaceSizeAndAlignment(&mTexture->surface);
    GX2InitTextureRegs(mTexture.get());

    if (mTexture->surface.imageSize == 0) {
        throw std::runtime_error{"Texture is empty"};
    }

    mTexture->surface.image = std::aligned_alloc(mTexture->surface.alignment,
                                                 mTexture->surface.imageSize);
    if (!mTexture->surface.image) {
        throw std::runtime_error{"Failed to allocate surface for texture"};
    }
}

Texture::~Texture() noexcept {
    if (mTexture) {
        std::free(mTexture->surface.image);
    }
}

GX2Texture *
Texture::get() noexcept {
    return mTexture.get();
}

void Texture::flush() noexcept {
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE,
                  getPixels(),
                  getSize());
}

void *Texture::getPixels() noexcept {
    return mTexture->surface.image;
}

std::size_t Texture::getSize() const noexcept {
    return mTexture->surface.imageSize;
}

std::uint32_t Texture::getRowPitch() noexcept {
    return mTexture->surface.pitch;
}

std::uint32_t Texture::getRowStride() noexcept {
    return getRowPitch() * 4;
}

uint32_t *Texture::getRow(uint32_t y) noexcept {
    uint32_t *pixels = reinterpret_cast<uint32_t *>(getPixels());
    return &pixels[y * getRowPitch()];
}

Texture::operator bool() const noexcept {
    return !!mTexture;
}
