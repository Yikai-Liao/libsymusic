//
// Created by lyk on 23-12-17.
//
#include <cstdio>
#include <stdexcept>
#include "symusic/inner/io.h"

namespace symusic::io {

vec<u8> read_file(const std::filesystem::path& path) {
    if(!exists(path)) { throw std::runtime_error("File not found"); }
    FILE * file = fopen(path.string().c_str(), "rb");
    if(!file) { throw std::runtime_error("File not found"); }
    fseek(file, 0, SEEK_END);
    const auto size = ftell(file);
    fseek(file, 0, SEEK_SET);
    vec<u8> buffer(size);
    fread(buffer.data(), 1, size, file);
    fclose(file);
    return buffer;
}

void write_file(const std::filesystem::path& path, const std::span<const u8> buffer) {
    FILE * file = fopen(path.string().c_str(), "wb");
    if(!file) { throw std::runtime_error("File not found"); }
    fwrite(buffer.data(), 1, buffer.size(), file);
    fclose(file);

}

}