#include "utils.h"
#include "logger.h"
#include <coreinit/time.h>
#include <cstdio>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std::literals;

std::expected<std::vector<std::byte>, std::string> LoadFile(const std::filesystem::path &filename) noexcept {
    try {
        if (!exists(filename)) {
            return std::unexpected{filename.string() + " does not exist."s};
        }
        auto size = file_size(filename);
        std::vector<std::byte> buffer(size);

        FILE *f = std::fopen(filename.c_str(), "rb");
        if (!f) {
            return std::unexpected{"Could not open "s + filename.string()};
        }

        auto read = std::fread(buffer.data(), 1, size, f);
        std::fclose(f);
        if (read < size) {
            return std::unexpected{"Could not read the whole file."};
        }

        return {std::move(buffer)};
    } catch (std::exception &e) {
        return std::unexpected{"Could not load file: "s + e.what()};
    }
}

namespace {
    std::optional<std::minstd_rand> sRandomEngine;
} // namespace

std::size_t GetRandomIndex(std::size_t size) {
    if (!sRandomEngine) {
        auto t = static_cast<std::uint64_t>(OSGetTime());
        std::seed_seq seeder{static_cast<std::uint32_t>(t),
                             static_cast<std::uint32_t>(t >> 32)};
        sRandomEngine.emplace(seeder);
    }
    std::uniform_int_distribution<std::size_t> dist{0, size - 1};
    return dist(*sRandomEngine);
}

std::filesystem::path ToLower(const std::filesystem::path &p) {
    std::string result;
    for (auto c : p.string()) {
        result.push_back(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}
