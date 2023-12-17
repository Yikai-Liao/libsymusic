//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_ALIAS_H
#define LIBSYMUSIC_ALIAS_H

#include <cstdint>
#include <vector>

namespace symusic {
// Simple Type Definitions Begin
typedef uint8_t     u8;
typedef int8_t      i8;
typedef uint16_t    u16;
typedef int16_t     i16;
typedef uint32_t    u32;
typedef int32_t     i32;
typedef float       f32;
typedef double      f64;
using size_t = std::size_t;

template<typename T>
using vec = std::vector<T>;

}


#endif //LIBSYMUSIC_ALIAS_H
