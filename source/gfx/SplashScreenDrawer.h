#pragma once

#include "ShaderSerializer.h"
#include "gfx.h"
#include <filesystem>
#include <gx2/sampler.h>
#include <gx2/shaders.h>
#include <gx2/texture.h>
#include <gx2r/buffer.h>
#include <memory>

class SplashScreenDrawer {
public:
    explicit SplashScreenDrawer(const std::filesystem::path &meta_dir);

    void Draw();

    virtual ~SplashScreenDrawer();

private:
    const float mPositionData[8] = {
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
    };

    const float mTexCoords[8] = {
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
    };

    WHBGfxShaderGroup mShaderGroup = {};
    std::unique_ptr<GX2VertexShaderWrapper> mVertexShaderWrapper;
    std::unique_ptr<GX2PixelShaderWrapper> mPixelShaderWrapper;
    GX2RBuffer mPositionBuffer = {};
    GX2RBuffer mTexCoordBuffer = {};
    GX2Texture *mTexture       = nullptr;
    GX2Sampler mSampler        = {};
};
