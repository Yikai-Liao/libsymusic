//
// Created by lyk on 2023/12/24.
//

#ifndef LIBSYMUSIC_IO_COMMON_H
#define LIBSYMUSIC_IO_COMMON_H

#include <string>
#include <filesystem>

namespace symusic {

vec<u8> read_file(const std::filesystem::path & path);

inline vec<u8> read_file(const std::string & path) {
    return read_file(std::filesystem::path(path));
}

void write_file(const std::filesystem::path & path, std::span<const u8> buffer);

inline void write_file(const std::string & path, std::span<const u8> buffer) {
    write_file(std::filesystem::path(path), buffer);
}

}

#endif //LIBSYMUSIC_IO_COMMON_H
