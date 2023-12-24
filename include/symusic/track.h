//
// Created by lyk on 2023/12/24.
//

#ifndef LIBSYMUSIC_TRACK_H
#define LIBSYMUSIC_TRACK_H

#include <span>

#include "symusic/io/iodef.h"
#include "symusic/event.h"

namespace symusic {
template<TType T>
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

    Track() = default;

    Track(const Track &) = default;

    [[nodiscard]] Track copy() const { return {*this}; }

    Track& operator=(const Track &) = default;

    Track(std::string name, const u8 program, const bool is_drum):
        name{std::move(name)}, program{program}, is_drum{is_drum} {}

    Track(
        std::string name,const u8 program, const bool is_drum,
        const vec<Note<T>> & notes, const vec<ControlChange<T>> & controls,
        const vec<PitchBend<T>> & pitch_bends, const vec<Pedal<T>> & pedals
    ):  name{std::move(name)}, program{program}, is_drum{is_drum},
        notes{notes}, controls{controls}, pitch_bends{pitch_bends}, pedals{pedals} {}

    template<DataFormat F>
    [[nodiscard]] static Track parse(std::span<const u8> bytes);

    template<DataFormat F>
    [[nodiscard]] vec<u8> dumps() const;


    // return the start time of the track
    [[nodiscard]] unit start() const;

    // return the end time of the track
    [[nodiscard]] unit end() const;

    // return the number of notes in the track
    [[nodiscard]] size_t note_num() const;

    // return true if the track is empty
    [[nodiscard]] bool empty() const;

    // return a string representation of the track
    [[nodiscard]] std::string to_string() const;

    // summary info of the track
    [[nodiscard]] std::string summary() const;

    // non-inplace sort, return a new Track
    [[nodiscard]] Track sort(bool reverse = false);

    // inplace sort, and return self reference
    Track& sort_inplace(bool reverse = false);

    // Clip all the events in the track, non-inplace, return a new Track
    // For events with duration, clip_end is used to determine whether to clip based on end time.
    [[nodiscard]] Track clip(unit start, unit end, bool clip_end = false) const;
};

// "Not Implemented" Error at compile time for parse and dumps

template<TType T> template<DataFormat>
Track<T> Track<T>::parse(std::span<const u8>) {
    static_assert(true, "Not implemented"); return {};
}

template<TType T> template<DataFormat>
vec<u8> Track<T>::dumps() const {
    static_assert(true, "Not implemented"); return {};
}

}

#endif //LIBSYMUSIC_TRACK_H
