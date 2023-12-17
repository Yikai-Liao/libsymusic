//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_ZPP_H
#define LIBSYMUSIC_ZPP_H

#include "symusic/inner/zpp.h"
#include "symusic/inner/io.h"

namespace symusic::io {

#define INSTANTIATE_ZPP(__COUNT, NAME)                                                 \
    extern template vec<u8> serailize_zpp(const NAME<tag::Tick> & data);               \
    extern template vec<u8> serailize_zpp(const NAME<tag::Quarter> & data);            \
    extern template vec<u8> serailize_zpp(const NAME<tag::Second> & data);             \
    extern template NAME<tag::Tick> deserailize_zpp(std::span<const u8> buffer);       \
    extern template NAME<tag::Quarter> deserailize_zpp(std::span<const u8> buffer);    \
    extern template NAME<tag::Second> deserailize_zpp(std::span<const u8> buffer);

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

// extern parse and dumps
#define INSTANTIATE_ZPP(__COUNT, TAG)                                                       \
    extern template vec<u8> dumps<tag::TAG, tag::ZPP>(const Score<tag::TAG> & score);       \
    extern template Score<tag::TAG> parse<tag::TAG, tag::ZPP>(std::span<const u8> buffer);

REPEAT_ON(INSTANTIATE_ZPP, Tick, Quarter, Second)

#undef INSTANTIATE_ZPP
}

#endif //LIBSYMUSIC_ZPP_H
