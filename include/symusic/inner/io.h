//
// Created by lyk on 23-12-17.
//

#ifndef LIBSYMUSIC_INNER_IO_H
#define LIBSYMUSIC_INNER_IO_H

#include "symusic/container.h"
#include <filesystem>
#include <span>

namespace symusic::io {
template<trait::TType T, trait::Format F>
Score<T> parse(std::span<const u8> buffer);

template<trait::TType T, trait::Format F>
vec<u8> dumps(const Score<T> & score);

#undef EXTERN_METHODS

vec<u8> read_file(const std::filesystem::path & path);

void write_file(const std::filesystem::path & path, std::span<const u8> buffer);

template<typename Iter>
void write_file(const std::filesystem::path & path, Iter begin, Iter end) {
    static_assert(
        std::is_same_v<typename std::iterator_traits<Iter>::value_type, u8> ||
        std::is_same_v<typename std::iterator_traits<Iter>::value_type, i8>
    );
    write_file(path, std::span(
        reinterpret_cast<const u8*>(&*begin),
        reinterpret_cast<const u8*>(&*end)
    ));
}

template<trait::TType T, trait::Format F>
Score<T> parse(const std::filesystem::path & path) {
    const vec<u8> buffer = read_file(path);
    return parse<T, F>(std::span(buffer));
}

template<trait::TType T, trait::Format F, typename Iter>
Score<T> parse(Iter begin, Iter end) {
    static_assert(
        std::is_same_v<typename std::iterator_traits<Iter>::value_type, u8> ||
        std::is_same_v<typename std::iterator_traits<Iter>::value_type, i8>
    );
    return parse<T, F>(std::span(
        reinterpret_cast<const u8*>(&*begin),
        reinterpret_cast<const u8*>(&*end)
    ));
}

template<trait::TType T, trait::Format F>
void dump(const Score<T> & score, const std::filesystem::path & path) {
    const vec<u8> buffer = dumps<T, F>(score);
    write_file(path, std::span(buffer));
}
}
#endif //LIBSYMUSIC_INNER_IO_H
