#include "TGATexture.h"
#include <cstring>
#include <stdexcept>
#include <utility>

using namespace std::literals;

/*
 * Based on
 * https://github.com/Xpl0itU/savemii/blob/70e3b63db52113519230e1e39bd56876cef12dc8/src/tga_reader.cpp
 * and
 * https://github.com/Crementif/WiiU-GX2-Shader-Examples/blob/5a88f861043dcb7666d4d25a6bab6bd271e76d5f/include/TGATexture.h
 */

struct WUT_PACKED TGA_HEADER {
    uint8_t identsize;     // size of ID field that follows 18 byte header (0 usually)
    uint8_t colourmaptype; // type of colour map 0=none, 1=has palette
    uint8_t imagetype;     // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    uint8_t colourmapstart[2];  // first colour map entry in palette
    uint8_t colourmaplength[2]; // number of colours in palette
    uint8_t colourmapbits;      // number of bits per palette entry 15,16,24,32

    uint16_t xstart;    // image x origin
    uint16_t ystart;    // image y origin
    uint16_t width;     // image width in pixels
    uint16_t height;    // image height in pixels
    uint8_t bits;       // image bits per pixel 8,16,24,32
    uint8_t descriptor; // image descriptor bits (vh flip bits)
};

uint16_t inline _swapU16(uint16_t v) {
    return (v >> 8) | (v << 8);
}

std::expected<Texture, std::string> TGA_LoadTexture(std::span<const uint8_t> data) noexcept {
    try {
        TGA_HEADER tgaHeader;
        if (data.size() < sizeof tgaHeader)
            throw std::runtime_error{"Truncated TGA image"};
        std::memcpy(&tgaHeader, data.data(), sizeof tgaHeader);

        uint32_t width  = _swapU16(tgaHeader.width);
        uint32_t height = _swapU16(tgaHeader.height);

        if (tgaHeader.bits != 24) {
            throw std::runtime_error{"Only 24bit TGA images are supported"};
        }
        if (tgaHeader.imagetype != 2 && tgaHeader.imagetype != 3) {
            throw std::runtime_error{"Only uncompressed TGA images are supported"};
        }

        Texture texture{width, height};

        for (uint32_t y = 0; y < height; y++) {
            uint32_t *row = texture.getRow(y);
            for (uint32_t x = 0; x < width; x++) {
                size_t index = sizeof(TGA_HEADER) + (3 * width * (height - 1 - y)) + (3 * x);

                int b = data[index + 0] & 0xFF;
                int g = data[index + 1] & 0xFF;
                int r = data[index + 2] & 0xFF;

                row[x] = r << 24 | g << 16 | b << 8 | 0xFF;
            }
        }

        // todo: create texture with optimal tile format and use GX2CopySurface to convert from linear to tiled format

        texture.flush();
        return texture;
    } catch (std::exception &e) {
        return std::unexpected{"[TGATexture] "s + e.what()};
    }
}
