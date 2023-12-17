//
// Created by lyk on 23-12-14.
//
#include "symusic/inner/io.h"
#include "symusic/inner/zpp.h"
#include "symusic/container.h"
#include "zpp_bits.h"


namespace zpp::bits {
#define SERIALIZE_NON_HEAP(__COUNT, NAME)                                       \
    template<symusic::trait::TType T>                                           \
    constexpr auto serialize(auto & archive, const symusic::NAME<T> & data) {   \
        return archive(zpp::bits::as_bytes(data));                              \
    }                                                                           \
    template<symusic::trait::TType T>                                           \
    constexpr auto serialize(auto & archive, symusic::NAME<T> & data) {         \
        return archive(zpp::bits::as_bytes(data));                              \
    }

REPEAT_ON(
    SERIALIZE_NON_HEAP,
    Note,
    Pedal,
    ControlChange,
    TimeSignature,
    KeySignature,
    Tempo,
    PitchBend
)

#undef SERIALIZE_NON_HEAP

// TextMeta
template<symusic::trait::TType T>
constexpr auto serialize(auto & archive, const symusic::TextMeta<T> & data) {
    return archive(data.time, data.text);
}
template<symusic::trait::TType T>
constexpr auto serialize(auto & archive, symusic::TextMeta<T> & data) {
    return archive(data.time, data.text);
}

// NoteArray
template<symusic::trait::TType T>
constexpr auto serialize(auto & archive, const symusic::NoteArray<T> & data) {
    return archive(
        data.name, data.program, data.is_drum,
        data.time, data.duration, data.pitch, data.velocity
    );
}
template<symusic::trait::TType T>
constexpr auto serialize(auto & archive, symusic::NoteArray<T> & data) {
    return archive(
        data.name, data.program, data.is_drum,
        data.time, data.duration, data.pitch, data.velocity
    );
}

// Track
template<symusic::trait::TType T>
constexpr auto serialize(auto & archive, const symusic::Track<T> & data) {
    return archive(
        data.name, data.program, data.is_drum, data.notes,
        data.controls, data.pitch_bends, data.pedals
    );
}
template<symusic::trait::TType T>
constexpr auto serialize(auto & archive, symusic::Track<T> & data) {
    return archive(
        data.name, data.program, data.is_drum, data.notes,
        data.controls, data.pitch_bends, data.pedals
    );
}

// Score
template<symusic::trait::TType T>
constexpr auto serialize(auto & archive, const symusic::Score<T> & data) {
    return archive(
        data.ticks_per_quarter, data.tracks, data.time_signatures,
        data.key_signatures, data.tempos, data.lyrics, data.markers
    );
}
template<symusic::trait::TType T>
constexpr auto serialize(auto & archive, symusic::Score<T> & data) {
    return archive(
        data.ticks_per_quarter, data.tracks, data.time_signatures,
        data.key_signatures, data.tempos, data.lyrics, data.markers
    );
}
}

namespace symusic::io {

template<class T>
vec<u8> serailize_zpp(const T & data) {
    vec<u8> buffer;
    auto out = zpp::bits::out(buffer);
    out(data).or_throw();
    return buffer;
}

template<class T>
T deserailize_zpp(std::span<const u8> buffer) {
    auto in = zpp::bits::in(buffer);
    T data{};
    in(data).or_throw();
    return data;
}

#define INSTANTIATE_ZPP(__COUNT, NAME)                                          \
    template vec<u8> serailize_zpp(const NAME<tag::Tick> & data);               \
    template vec<u8> serailize_zpp(const NAME<tag::Quarter> & data);            \
    template vec<u8> serailize_zpp(const NAME<tag::Second> & data);             \
    template NAME<tag::Tick> deserailize_zpp(std::span<const u8> buffer);       \
    template NAME<tag::Quarter> deserailize_zpp(std::span<const u8> buffer);    \
    template NAME<tag::Second> deserailize_zpp(std::span<const u8> buffer);

REPEAT_ON(
    INSTANTIATE_ZPP,
    Note,
    Pedal,
    ControlChange,
    TimeSignature,
    KeySignature,
    Tempo,
    PitchBend,
    TextMeta,
    NoteArray,
    Track,
    Score
)
#undef INSTANTIATE_ZPP

#define INSTANTIATE_ZPP(__COUNT, TAG)                                           \
    template<>                                                                  \
    vec<u8> dumps<tag::TAG, tag::ZPP>(const Score<tag::TAG> & score) {          \
        return serailize_zpp(score);                                            \
    }                                                                           \
    template<>                                                                  \
    Score<tag::TAG> parse<tag::TAG, tag::ZPP>(std::span<const u8> buffer) {     \
        return deserailize_zpp<Score<tag::TAG>>(buffer);                        \
    }

REPEAT_ON(INSTANTIATE_ZPP, Tick, Quarter, Second)

#undef INSTANTIATE_ZPP
}   // namespace symusic::io