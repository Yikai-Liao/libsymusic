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

namespace fmt {

#define TAG_FORMAT(__COUNT, NAME)                                                   \
template<>                                                                          \
struct formatter<symusic::tag::NAME> {                                              \
    constexpr auto parse(const format_parse_context &ctx) { return ctx.begin(); }   \
    template<typename FmtCtx>                                                       \
    auto format(const symusic::tag::NAME & _, FmtCtx &ctx) const {                  \
        return fmt::format_to(ctx.out(), #NAME);                                    \
    }                                                                               \
};

REPEAT_ON(TAG_FORMAT, Tick, Quarter, Second, MIDI, ZPP)

#undef TAG_FORMAT

}

namespace symusic {
template<trait::TType T>
std::string sumary(const Track<T> &track) {
    return fmt::format(
        "Track(ttype={}, program={}, is_drum={}, name={}, notes={})",
        T(), track.program, track.is_drum, track.name, track.notes.size()
    );
}

template<trait::TType T>
std::string sumary(const NoteArray<T> &note_array) {
    return fmt::format(
        "NoteArray(ttype={}, program={}, is_drum={}, name={}, notes={})",
        T(), note_array.program, note_array.is_drum, note_array.name, note_array.note_num()
    );
}

template<trait::TType T>
std::string sumary(const Score<T> &score) {
    return fmt::format(
        "Score(ttype={}, tpq={}, begin={}, end={}, tracks={}, notes={}, time_sig={}, key_sig={}, markers={}, lyrics={})",
        T(), score.ticks_per_quarter, score.start(), score.end(), score.tracks.size(), score.note_num(),
        score.time_signatures.size(), score.key_signatures.size(), score.markers.size(), score.lyrics.size()
    );
}

namespace details {struct BaseParser{};
}

}

namespace fmt {

template<>
struct formatter<symusic::details::BaseParser> {
    char presentation = 's';

    constexpr auto parse(const format_parse_context &ctx) {
        auto it = ctx.begin();
        auto const end = ctx.end();
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

// T is numeric type
template<typename T> requires std::is_arithmetic_v<T>
constexpr std::string fix_float(const T value) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return fmt::format("{:.2f}", value);
    }   return fmt::format("{}", value);
}

FORMATTER(
    Note, d,
    "Note(time={}, duration={}, pitch={}, velocity={})",
    "Note({}, {}, {}, {})",
    fix_float(d.time), fix_float(d.duration), d.pitch, d.velocity
)

FORMATTER(
    Pedal, d,
    "Pedal(time={}, duration={})",
    "Pedal({}, {})",
    fix_float(d.time), d.duration
)

FORMATTER(
    ControlChange, d,
    "ControlChange(time={}, number={}, value={})",
    "ControlChange({}, {}, {})",
    fix_float(d.time), d.number, d.value
)

FORMATTER(
    TimeSignature, d,
    "TimeSignature(time={}, numerator={}, denominator={})",
    "TimeSignature({}, {}, {})",
    fix_float(d.time), d.numerator, d.denominator
)

FORMATTER(
    KeySignature, d,
    "KeySignature(time={}, key={}, tonality={}, degree={})",
    "KeySignature({}, {}, {}, {})",
    fix_float(d.time), d.key, d.tonality, d.degree()
)

FORMATTER(
    Tempo, d,
    "Tempo(time={}, qpm={})",
    "Tempo({}, {})",
    fix_float(d.time), d.qpm
)

FORMATTER(
    PitchBend, d,
    "PitchBend(time={}, value={})",
    "PitchBend({}, {})",
    fix_float(d.time), d.value
)

FORMATTER(
    TextMeta, d,
    "Text(time={}, text=\"{}\")",
    "Text({}, \"{}\")",
    fix_float(d.time), d.text
)
#undef FORMATTER
#undef INNER_FORMATTER
#undef HELPER

#define SUMMARY_FORMAT(__COUNT, NAME) \
    template<symusic::trait::TType T> \
    struct formatter<symusic::NAME<T>> { \
        constexpr auto parse(const format_parse_context &ctx) { return ctx.begin(); } \
        template<typename FmtCtx> \
        auto format(const symusic::NAME<T> & data, FmtCtx &ctx) const { \
            return fmt::format_to(ctx.out(), "{}", symusic::sumary(data)); \
        } \
    };

REPEAT_ON(SUMMARY_FORMAT, Track, NoteArray, Score)

#undef SUMMARY_FORMAT
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
    Note,
    Pedal,
    ControlChange,
    TimeSignature,
    KeySignature,
    Tempo,
    PitchBend,
    TextMeta
)
#undef OSTREAMEABLE

#define OSTREAMEABLE(__COUNT, STRUCT_NAME)                                              \
    template<symusic::trait::TType T>                                                   \
    std::ostream &operator<<(std::ostream &os, const symusic::STRUCT_NAME<T> &data){    \
        return os << fmt::format("{}", data);                                           \
    }                                                                                   \
    namespace symusic {                                                                 \
    template<trait::TType T>                                                            \
    std::string to_string (const symusic::STRUCT_NAME<T> &data){                        \
        return fmt::format("{}", data);                                                 \
    }}

REPEAT_ON( OSTREAMEABLE, Track, NoteArray, Score)
#undef OSTREAMEABLE

#define OSTREAMEABLE(__COUNT, STRUCT_NAME)                                              \
    template<symusic::trait::TType T>                                                   \
    std::ostream &operator<<(                                                           \
        std::ostream &os, const std::vector<symusic::STRUCT_NAME<T>> &data              \
    ) { return os << fmt::format("{}", data); }                                         \
    namespace symusic {                                                                 \
    template<trait::TType T>                                                            \
    std::string to_string (const std::vector<symusic::STRUCT_NAME<T>> &data){           \
        return fmt::format("{}", data);                                                 \
    }}

REPEAT_ON(
    OSTREAMEABLE,
    Note,
    Pedal,
    ControlChange,
    TimeSignature,
    KeySignature,
    Tempo,
    PitchBend,
    TextMeta,
    Track
)
#undef OSTREAMEABLE

// define a base formatter with parse that
#endif //LIBSYMUSIC_REPR_H
