//
// Created by lyk on 23-12-14.
//
#include "symusic/container.h"
#include "symusic/format/zpp.h"


namespace symusic::details {
template<trait::TType T>
Score<T> parse_zpp(std::span<u8> buffer) {
    vec<u8> data(buffer.begin(), buffer.end());
    auto in = zpp::bits::in(data);
    Score<T> s;
    in(s).or_throw();
    return s;
}

template<trait::TType T>
vec<u8> dumps_zpp(const Score<T> &score) {
    vec<u8> buffer;
    auto out = zpp::bits::out(buffer);
    out(score).or_throw();
    return buffer;
}
}  // symusic