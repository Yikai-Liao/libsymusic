//
// Created by lyk on 23-12-14.
//
#ifndef LIBSYMUSIC_REPR_H
#define LIBSYMUSIC_REPR_H

// for ostream
#include "symusic/container.h"
#include <string>
#include <iosfwd>
#include "fmt/core.h"
#include "fmt/ranges.h"

// define fmt::formatter for symusic::Note
// support different format for Note
// :d for details Note(time=0.22, duration=0.51, pitch=65, velocity=0.5)
// :s for short Note(0.22, 0.51, 65, 0.5)
// if nothing, use short format, i.e. "{}" is the same as "{:s}"

namespace symusic::details {
    struct BaseParser{};
}

namespace fmt {

template<>
struct formatter<symusic::details::BaseParser> {
    char presentation = 's';

    constexpr auto parse(format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'd' || *it == 's')) {
            presentation = *it++;
        }
        if (it != end && *it != '}') {
            throw format_error("invalid format");
        }
        return it;
    }
};

#define INNER_FORMATTER(                                                            \
    STRUCT_NAME, ARG_NAME, DETAIL_FORMAT, SHORT_FORMAT, ...)                        \
    template<symusic::trait::TType T>                                               \
    struct formatter<symusic::STRUCT_NAME<T>>:                                      \
        formatter<symusic::details::BaseParser> {                                   \
        template<typename FmtCtx>                                                   \
        auto format(const symusic::STRUCT_NAME<T> &ARG_NAME, FmtCtx &ctx) const {   \
            if (this->presentation == 'd') {                                        \
                return fmt::format_to( ctx.out(), DETAIL_FORMAT, ##__VA_ARGS__ );   \
            } else {                                                                \
                return fmt::format_to( ctx.out(), SHORT_FORMAT, ##__VA_ARGS__ );    \
            }                                                                       \
        }                                                                           \
    };

// this is to make msvc work
#define HELPER(X) X
#define FORMATTER(...) HELPER(INNER_FORMATTER(__VA_ARGS__))

FORMATTER(
    Note, d,
    "Note(time={}, duration={}, pitch={}, velocity={})",
    "Note({}, {}, {}, {})",
    d.time, d.duration, d.pitch, d.velocity
)

FORMATTER(
    Pedal, d,
    "Pedal(time={}, duration={})",
    "Pedal({}, {})",
    d.time, d.duration
)

FORMATTER(
    ControlChange, d,
    "ControlChange(time={}, number={}, value={})",
    "ControlChange({}, {}, {})",
    d.time, d.number, d.value
)

FORMATTER(
    TimeSignature, d,
    "TimeSignature(time={}, numerator={}, denominator={})",
    "TimeSignature({}, {}, {})",
    d.time, d.numerator, d.denominator
)

FORMATTER(
    KeySignature, d,
    "KeySignature(time={}, key={}, tonality={}, degree={})",
    "KeySignature({}, {}, {}, {})",
    d.time, d.key, d.tonality, d.degree()
)

FORMATTER(
    Tempo, d,
    "Tempo(time={}, qpm={})",
    "Tempo({}, {})",
    d.time, d.qpm
)

FORMATTER(
    PitchBend, d,
    "PitchBend(time={}, value={})",
    "PitchBend({}, {})",
    d.time, d.value
)

FORMATTER(
    TextMeta, d,
    "Text(time={}, text=\"{}\")",
    "Text({}, \"{}\")",
    d.time, d.text
)
#undef FORMATTER
#undef INNER_FORMATTER
#undef HELPER
}

#define OSTREAMEABLE(__COUNT, STRUCT_NAME)                                              \
    template<symusic::trait::TType T>                                                   \
    std::ostream &operator<<(std::ostream &os, const symusic::STRUCT_NAME<T> &data){    \
        return os << fmt::format("{:d}", data);                                         \
    }                                                                                   \
    namespace symusic {                                                                 \
    template<trait::TType T>                                                            \
    std::string to_string (const symusic::STRUCT_NAME<T> &data){                        \
        return fmt::format("{:d}", data);                                               \
    }}

REPEAT_ON(
    OSTREAMEABLE,
    Note, Pedal, ControlChange, TimeSignature, KeySignature, Tempo, PitchBend, TextMeta
)
#undef OSTREAMEABLE
// define a base formatter with parse that
#endif //LIBSYMUSIC_REPR_H
