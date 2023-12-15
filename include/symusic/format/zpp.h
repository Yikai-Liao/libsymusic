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

template<trait::TType T>
vec<u8> dumps_zpp(const Score<T> &score);
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

// these serialize functions are forced to be writen in header to pass compilation
// I don't know why
namespace zpp::bits {
template<symusic::trait::TType T> // Note
constexpr auto serialize(auto &archive, symusic::Note<T> &self) {
    return archive(self.time, self.duration, self.pitch, self.velocity);
}

template<symusic::trait::TType T> // Pedal
constexpr auto serialize(auto &archive, symusic::Pedal<T> &self) {
    return archive(self.time, self.duration);
}

template<symusic::trait::TType T> // ControlChange
constexpr auto serialize(auto &archive, symusic::ControlChange<T> &self) {
    return archive(self.time, self.number, self.value);
}

template<symusic::trait::TType T> // TimeSignature
constexpr auto serialize(auto &archive, symusic::TimeSignature<T> &self) {
    return archive(self.time, self.numerator, self.denominator);
}

template<symusic::trait::TType T> // KeySignature
constexpr auto serialize(auto &archive, symusic::KeySignature<T> &self) {
    return archive(self.time, self.key, self.tonality);
}

template<symusic::trait::TType T> // Tempo
constexpr auto serialize(auto &archive, symusic::Tempo<T> &self) {
    return archive(self.time, self.qpm);
}

template<symusic::trait::TType T> // PitchBend
constexpr auto serialize(auto &archive, symusic::PitchBend<T> &self) {
    return archive(self.time, self.value);
}

template<symusic::trait::TType T> // TextMeta
constexpr auto serialize(auto &archive, symusic::TextMeta<T> &self) {
    return archive(self.time, self.text);
}

template<symusic::trait::TType T> // Track
constexpr auto serialize(auto &archive, symusic::Track<T> &self) {
    return archive(
        self.name, self.program,self.is_drum,
        self.notes, self.controls, self.pitch_bends, self.pedals
    );
}

template<symusic::trait::TType T> // Score
constexpr auto serialize(auto &archive, symusic::Score<T> &self) {
    return archive(
        self.ticks_per_quarter, self.tracks,
        self.time_signatures, self.key_signatures,
        self.tempos, self.lyrics, self.markers
    );
}
}
#endif //LIBSYMUSIC_ZPP_H
