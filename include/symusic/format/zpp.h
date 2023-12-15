//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_ZPP_H
#define LIBSYMUSIC_ZPP_H

#include "symusic/format/common.h"
#include "symusic/container.h"
#include "zpp_bits.h"

namespace symusic {
namespace tag {
// set a none type
struct Zpp: FormatTag<Empty>{};
}

namespace details {
template<trait::TType T>
Score<T> parse_zpp(std::span<u8> buffer);
}

template<> template<>
inline Score<tag::Tick> Score<tag::Tick>::from<tag::Zpp>(const std::span<u8> buffer) {
    return details::parse_zpp<tag::Tick>(buffer);
}

template<> template<>
inline Score<tag::Quarter> Score<tag::Quarter>::from<tag::Zpp>(const std::span<u8> buffer) {
    return details::parse_zpp<tag::Quarter>(buffer);
}

template<> template<>
inline Score<tag::Second> Score<tag::Second>::from<tag::Zpp>(const std::span<u8> buffer) {
    return details::parse_zpp<tag::Second>(buffer);
}

}  // symusic
#endif //LIBSYMUSIC_ZPP_H
