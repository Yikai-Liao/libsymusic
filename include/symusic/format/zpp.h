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
Score<T> parse_zpp(std::span<const u8> buffer);

template<trait::TType T>
vec<u8> dumps_zpp(const Score<T> &score);
}

template<> template<>
inline Score<tag::Tick> Score<tag::Tick>::from<tag::Zpp>(const std::span<const u8> buffer) {
    return details::parse_zpp<tag::Tick>(buffer);
}

template<> template<>
inline Score<tag::Quarter> Score<tag::Quarter>::from<tag::Zpp>(const std::span<const u8> buffer) {
    return details::parse_zpp<tag::Quarter>(buffer);
}

template<> template<>
inline Score<tag::Second> Score<tag::Second>::from<tag::Zpp>(const std::span<const u8> buffer) {
    return details::parse_zpp<tag::Second>(buffer);
}

template<> template<>
inline vec<u8> Score<tag::Tick>::dumps<tag::Zpp>() const {
    return details::dumps_zpp(*this);
}

template<> template<>
inline vec<u8> Score<tag::Quarter>::dumps<tag::Zpp>() const {
    return details::dumps_zpp(*this);
}

template<> template<>
inline vec<u8> Score<tag::Second>::dumps<tag::Zpp>() const {
    return details::dumps_zpp(*this);
}

}  // symusic


// these serialize functions are forced to be writen in header to pass compilation
// I don't know why
namespace symusic {
template<trait::TType T> // Note
constexpr auto serialize(auto &archive, Note<T> &self) {
    return archive(self.time, self.duration, self.pitch, self.velocity);
}

template<trait::TType T> // Pedal
constexpr auto serialize(auto &archive, Pedal<T> &self) {
    return archive(self.time, self.duration);
}

template<trait::TType T> // ControlChange
constexpr auto serialize(auto &archive, ControlChange<T> &self) {
    return archive(self.time, self.number, self.value);
}

template<trait::TType T> // TimeSignature
constexpr auto serialize(auto &archive, TimeSignature<T> &self) {
    return archive(self.time, self.numerator, self.denominator);
}

template<trait::TType T> // KeySignature
constexpr auto serialize(auto &archive, KeySignature<T> &self) {
    return archive(self.time, self.key, self.tonality);
}

template<trait::TType T> // Tempo
constexpr auto serialize(auto &archive, Tempo<T> &self) {
    return archive(self.time, self.qpm);
}

template<trait::TType T> // PitchBend
constexpr auto serialize(auto &archive, PitchBend<T> &self) {
    return archive(self.time, self.value);
}

template<trait::TType T> // TextMeta
constexpr auto serialize(auto &archive, TextMeta<T> &self) {
    return archive(self.time, self.text);
}

template<trait::TType T> // Track
constexpr auto serialize(auto &archive, Track<T> &self) {
    return archive(
        self.name, self.program,self.is_drum,
        self.notes, self.controls, self.pitch_bends, self.pedals
    );
}

template<trait::TType T> // Score
constexpr auto serialize(auto &archive, Score<T> &self) {
    return archive(
        self.ticks_per_quarter, self.tracks,
        self.time_signatures, self.key_signatures,
        self.tempos, self.lyrics, self.markers
    );
}
template<trait::TType T> // Note
constexpr auto serialize(auto &archive,const Note<T> &self) {
    return archive(self.time, self.duration, self.pitch, self.velocity);
}

template<trait::TType T> // Pedal
constexpr auto serialize(auto &archive,const Pedal<T> &self) {
    return archive(self.time, self.duration);
}

template<trait::TType T> // ControlChange
constexpr auto serialize(auto &archive,const ControlChange<T> &self) {
    return archive(self.time, self.number, self.value);
}

template<trait::TType T> // TimeSignature
constexpr auto serialize(auto &archive,const TimeSignature<T> &self) {
    return archive(self.time, self.numerator, self.denominator);
}

template<trait::TType T> // KeySignature
constexpr auto serialize(auto &archive,const KeySignature<T> &self) {
    return archive(self.time, self.key, self.tonality);
}

template<trait::TType T> // Tempo
constexpr auto serialize(auto &archive,const Tempo<T> &self) {
    return archive(self.time, self.qpm);
}

template<trait::TType T> // PitchBend
constexpr auto serialize(auto &archive,const PitchBend<T> &self) {
    return archive(self.time, self.value);
}

template<trait::TType T> // TextMeta
constexpr auto serialize(auto &archive,const TextMeta<T> &self) {
    return archive(self.time, self.text);
}

template<trait::TType T> // Track
constexpr auto serialize(auto &archive,const Track<T> &self) {
    return archive(
        self.name, self.program,self.is_drum,
        self.notes, self.controls, self.pitch_bends, self.pedals
    );
}

template<trait::TType T> // Score
constexpr auto serialize(auto &archive,const Score<T> &self) {
    return archive(
        self.ticks_per_quarter, self.tracks,
        self.time_signatures, self.key_signatures,
        self.tempos, self.lyrics, self.markers
    );
}
}
#endif //LIBSYMUSIC_ZPP_H
