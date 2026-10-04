#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <span>
#include <string>
#include <vector>

std::expected<std::vector<std::byte>, std::string> LoadFile(const std::filesystem::path &filename) noexcept;

std::size_t GetRandomIndex(std::size_t size);

std::filesystem::path ToLower(const std::filesystem::path &p);

// Helper to convert byte spans.
template<typename T, std::size_t E>
    requires(sizeof(T) == 1)
std::span<const T, E> ToSpan(std::span<const std::byte, E> input) {
    return std::span<const T, E>{reinterpret_cast<const T *>(input.data()), input.size()};
}
