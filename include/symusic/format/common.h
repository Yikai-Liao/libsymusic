//
// Created by lyk on 23-12-14.
//

#ifndef LIBSYMUSIC_FMT_COMMON_H
#define LIBSYMUSIC_FMT_COMMON_H

#include "symusic/container.h"

namespace symusic {

namespace details {
    inline vec<u8> read_file(const std::filesystem::path & path) {
        if(!exists(path)) { throw std::runtime_error("File not found"); }
        // use fread to load the file
        FILE * file = fopen(path.c_str(), "rb");
        if(!file) { throw std::runtime_error("File not found"); }
        fseek(file, 0, SEEK_END);
        const auto size = ftell(file);
        fseek(file, 0, SEEK_SET);
        vec<u8> buffer(size);
        fread(buffer.data(), 1, size, file);
        fclose(file);
        return buffer;
    }
}

// check the filepath and load
template<trait::TType T> template<trait::Format U>
Score<T> Score<T>::from(const std::filesystem::path & path) {
    const auto buffer = details::read_file(path);
    return Score::from<U>(std::span(buffer));
}

}
#endif //LIBSYMUSIC_FMT_COMMON_H
