//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_CONTAINER_H
#define LIBSYMUSIC_CONTAINER_H

#include <bits/stringfwd.h>
#include <filesystem>
#include <span>
#include "symusic/tag.h"

namespace symusic {
// Default Methods Macro
#define COMPILER_DEFAULT_METHODS(T)                             \
    T() = default;                                              \
    T(const T&) = default;                                      \
    T(T&&) noexcept = default;                                  \
    T& operator=(const T&) = default;                           \
    T& operator=(T&&) noexcept = default;                       \
    T copy() const { return {*this}; }

// Define Basic Declarations for TimeStamp
#define BASIC_TIMESTAMP_METHODS(EVENT, T)                       \
    COMPILER_DEFAULT_METHODS(EVENT)                             \
    typedef T ttype;                                            \
    typedef typename T::unit unit;                              \
    bool operator==(const EVENT & other) const;                 \
    [[nodiscard]] inline std::string to_string() const;         \
    inline EVENT& shift_time_inplace(unit offset);              \
    [[nodiscard]] inline EVENT shift_time(unit offset) const;   \

/*
 *  List of all the events (based on TimeStamp):
 *  - Note(duration: unit, pitch: i8, velocity: i8)
 *  - Pedal(duration: unit)
 *  - ControlChange(number: u8, value: u8)
 *  - TimeSignature(numerator: u8, denominator: u8)
 *  - KeySignature(key: i8, tonality: u8)
 *  - Tempo(qpm: f32)
 *  - PitchBend(value: i32)
 *  - TextMeta(text: string)
 */

template<trait::TType T>
struct Note: TimeStamp<T> {
    BASIC_TIMESTAMP_METHODS(Note, T);
    unit duration{0};
    i8 pitch{0}, velocity{0};

    Note(const unit time, const unit duration, const i8 pitch, const i8 velocity):
        TimeStamp<T>{time}, duration{duration}, pitch{pitch}, velocity{velocity} {}

    template<class U>
    Note(const unit time, const unit duration, const Note<U> & other):
        TimeStamp<T>{time}, duration{duration}, pitch{other.pitch}, velocity{other.velocity} {}

    [[nodiscard]] unit start() const;
    [[nodiscard]] unit end() const;

    Note& shift_pitch_inplace(i8 offset);
    Note& shift_velocity_inplace(i8 offset);

    Note shift_pitch(i8 offset) const;
    [[nodiscard]] Note shift_velocity(i8 offset) const;
};

template<trait::TType T>
struct Pedal: TimeStamp<T> {
    BASIC_TIMESTAMP_METHODS(Pedal, T);
    unit duration;

    Pedal(const unit time, const unit duration):
        TimeStamp<T>{time}, duration{duration} {}

    template<class U>
    Pedal(const unit time, const unit duration, const Pedal<U> & other):
        TimeStamp<T>{time}, duration{duration} {}

    [[nodiscard]] unit start();
    [[nodiscard]] unit end();

};

template<trait::TType T>
struct ControlChange: TimeStamp<T> {
    BASIC_TIMESTAMP_METHODS(ControlChange, T);
    u8 number{0}, value{0};

    ControlChange(const unit time, const u8 number, const u8 value):
        TimeStamp<T>{time}, number{number}, value{value} {}

    template<class U>
    ControlChange(const unit time, const ControlChange<U> & other):
        TimeStamp<T>{time}, number{other.number}, value{other.value} {}
};

template<trait::TType T>
struct TimeSignature: TimeStamp<T> {
    BASIC_TIMESTAMP_METHODS(TimeSignature, T);
    u8 numerator{4}, denominator{4};

    TimeSignature(const unit time, const u8 numerator, const u8 denominator):
        TimeStamp<T>{time}, numerator{numerator}, denominator{denominator} {}

    template<class U>
    TimeSignature(const unit time, const TimeSignature<U> & other):
        TimeStamp<T>{time}, numerator{other.numerator}, denominator{other.denominator} {}
};

template<trait::TType T>
struct KeySignature: TimeStamp<T> {
    BASIC_TIMESTAMP_METHODS(KeySignature, T);
    i8 key{0}, tonality{0};

    KeySignature(const unit time, const i8 key, const i8 tonality):
        TimeStamp<T>{time}, key{key}, tonality{tonality} {}

    template<class U>
    KeySignature(const unit time, const KeySignature<U> & other):
        TimeStamp<T>{time}, key{other.key}, tonality{other.tonality} {}

    [[nodiscard]] u8 degree() const;
};

template<trait::TType T>
struct Tempo: TimeStamp<T> {
    BASIC_TIMESTAMP_METHODS(Tempo, T);
    f32 qpm{120};

    Tempo(const unit time, const f32 qpm):
        TimeStamp<T>{time}, qpm{qpm} {}

    template<class U>
    Tempo(const unit time, const Tempo<U> & other):
        TimeStamp<T>{time}, qpm{other.qpm} {}
};

template<trait::TType T>
struct PitchBend: TimeStamp<T> {
    BASIC_TIMESTAMP_METHODS(PitchBend, T);
    i32 value{0};

    PitchBend(const unit time, const i32 value):
        TimeStamp<T>{time}, value{value} {}

    template<class U>
    PitchBend(const unit time, const PitchBend<U> & other):
        TimeStamp<T>{time}, value{other.value} {}
};

template<trait::TType T>
struct TextMeta: TimeStamp<T> {
    BASIC_TIMESTAMP_METHODS(TextMeta, T);
    std::string text;

    TextMeta(const unit time, std::string  text):
        TimeStamp<T>{time}, text{std::move(text)} {}

    TextMeta(const unit time, std::string && text):
        TimeStamp<T>{time}, text{std::move(text)} {}

    template<class U>
    TextMeta(const unit time, const TextMeta<U> & other):
        TimeStamp<T>{time}, text{other.text} {}
};

#undef BASIC_TIMESTAMP_METHODS

template<trait::TType T>
struct NoteArray {
    typedef T ttype;
    typedef typename T::unit unit;

    std::string name;
    u8 program = 0;
    bool is_drum = false;
    vec<unit> time, duration;
    vec<i8> pitch, velocity;

    COMPILER_DEFAULT_METHODS(NoteArray);

    NoteArray(std::string name, const u8 program, const bool is_drum):
        name{std::move(name)}, program{program}, is_drum{is_drum} {}

    NoteArray(std::string name, u8 program, bool is_drum, const vec<Note<T>> & notes);

    NoteArray(
        std::string name, const u8 program, const bool is_drum,
        const vec<unit> & time, const vec<unit> & duration,
        const vec<i8> & pitch, const vec<i8> & velocity
    ):  name{std::move(name)}, program{program}, is_drum{is_drum},
        time{time}, duration{duration}, pitch{pitch}, velocity{velocity} {}

    [[nodiscard]] unit start() const;
    [[nodiscard]] unit end() const;
    [[nodiscard]] size_t note_num() const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::string to_string() const;

    NoteArray& sort_inplace(bool reverse = false);
    [[nodiscard]] NoteArray sort(bool reverse = false) const;

    [[nodiscard]] NoteArray clip(unit start, unit end, bool clip_end = false) const;
};

template<trait::TType T>
struct Track {
    typedef T ttype;
    typedef typename T::unit unit;

    std::string name;
    u8 program = 0;
    bool is_drum = false;
    vec<Note<T>> notes;
    vec<ControlChange<T>> controls;
    vec<PitchBend<T>> pitch_bends;
    vec<Pedal<T>> pedals;

    COMPILER_DEFAULT_METHODS(Track);

    Track(std::string name, const u8 program, const bool is_drum):
        name{std::move(name)}, program{program}, is_drum{is_drum} {}

    Track(
        std::string name,const u8 program, const bool is_drum,
        const vec<Note<T>> & notes, const vec<ControlChange<T>> & controls,
        const vec<PitchBend<T>> & pitch_bends, const vec<Pedal<T>> & pedals
    ):  name{std::move(name)}, program{program}, is_drum{is_drum},
        notes{notes}, controls{controls}, pitch_bends{pitch_bends}, pedals{pedals} {}

    [[nodiscard]] unit start() const;
    [[nodiscard]] unit end() const;
    [[nodiscard]] size_t note_num() const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::string to_string() const;

    Track& sort_inplace(bool reverse = false);
    [[nodiscard]] Track sort(bool reverse = false) const;

    [[nodiscard]] Track clip(unit start, unit end, bool clip_end = false) const;
};

template<trait::TType T>
struct Score {
    typedef T ttype;
    typedef typename T::unit unit;

    i32 ticks_per_quarter{};
    vec<Track<T>> tracks;
    vec<TimeSignature<T>> time_signatures;
    vec<KeySignature<T>> key_signatures;
    vec<Tempo<T>> tempos;
    vec<TextMeta<T>> lyrics, markers;

    COMPILER_DEFAULT_METHODS(Score);

    explicit Score(const i32 tpq): ticks_per_quarter{tpq} {}

    Score(
        const i32 tpq, const vec<Track<T>> & tracks,
        const vec<TimeSignature<T>> & time_signatures,
        const vec<KeySignature<T>> & key_signatures,
        const vec<Tempo<T>> & tempos,
        const vec<Tempo<T>> & lyrics,
        const vec<Tempo<T>> & markers
    ):  ticks_per_quarter{tpq}, tracks{tracks}, time_signatures{time_signatures},
        key_signatures{key_signatures}, tempos{tempos}, lyrics{lyrics}, markers{markers} {}

    template<trait::TType U>    // from other score
    static Score from(const Score<U> & other);

    template<trait::TType U>
    Score<U> to();

    template<trait::Format U>   // from path(file)
    static Score from(const std::filesystem::path & path);

    template<trait::Format U>
    void dump(const std::filesystem::path & path);

    template<trait::Format U>   // from buffer
    static Score from(std::span<u8> buffer);

    template<trait::Format U>
    std::vector<u8> dumps();

    [[nodiscard]] unit start() const;
    [[nodiscard]] unit end() const;
    [[nodiscard]] size_t track_num() const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::string to_string() const;
};

#undef COMPILER_DEFAULT_METHODS

}   // namespace symusic
#endif //LIBSYMUSIC_CONTAINER_H