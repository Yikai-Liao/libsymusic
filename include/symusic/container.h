//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_CONTAINER_H
#define LIBSYMUSIC_CONTAINER_H

#include "symusic/inner/container.h"
#include "MetaMacro.h"

namespace symusic {

#define EXTERN_TEMPLATE(_COUNT, T)          \
    extern template struct T<tag::Tick>;    \
    extern template struct T<tag::Quarter>; \
    extern template struct T<tag::Second>;

REPEAT_ON(
    EXTERN_TEMPLATE,
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

#undef EXTERN_TEMPLATE
}   // namespace symusic
#endif //LIBSYMUSIC_CONTAINER_H
