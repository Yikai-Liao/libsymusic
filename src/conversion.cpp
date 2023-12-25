//
// Created by lyk on 23-12-16.
//
#include <cmath>

#include "symusic/conversion.h"

namespace symusic {

namespace details {

struct Tick2Quarter {
    float tpq;

    template<TType T>
    explicit Tick2Quarter(const Score<T> &score): tpq(static_cast<float>(score.ticks_per_quarter)) {}

    float operator()(const i32 tick) const {
        return static_cast<float>(tick) / tpq;
    }
};

struct Quarter2Tick {
    float tpq;

    template<TType T>
    explicit Quarter2Tick(const Score<T> &score): tpq(static_cast<float>(score.ticks_per_quarter)) {}

    i32 operator()(const float quarter) const {
        return static_cast<i32>(std::round(quarter * tpq));
    }
};
} // namespace details

#define HELPER_NAME(From, To) details::From##2##To

#define CONVERT_VEC_TIME(Name, In, Out, Convert)        \
    In.Name.reserve(Out.Name.size());                   \
    for(const auto & d : In.Name) {                     \
        In.Name.emplace_back(                           \
            Convert(d.time), d);                        \
    }

#define CONVERT_VEC_TIME_DUR(Name, In, Out, Convert)    \
    In.Name.reserve(Out.Name.size());                   \
    for(const auto & d : In.Name) {                     \
        In.Name.emplace_back(                           \
            Convert(d.time), Convert(d.duration), d     \
        );                                              \
    }

#define IMPLEMENT_CONVERT(To, From) \
    template<> Score<To> convert<To, From> (const Score<From> & score) {    \
        HELPER_NAME(From, To) helper(score);                                                    \
        Score<To> new_s(score.ticks_per_quarter);                                               \
        CONVERT_VEC_TIME(key_signatures, new_s, score, helper);                                 \
        CONVERT_VEC_TIME(time_signatures, new_s, score, helper);                                \
        CONVERT_VEC_TIME(tempos, new_s, score, helper);                                         \
        CONVERT_VEC_TIME(lyrics, new_s, score, helper);                                         \
        CONVERT_VEC_TIME(markers, new_s, score, helper);                                        \
        new_s.tracks.reserve(score.tracks.size());                                              \
        for(const Track<From> &track: score.tracks) {                                           \
            Track<To> new_t(track.name, track.program, track.is_drum);                          \
            CONVERT_VEC_TIME_DUR(notes, new_t, track, helper);                                  \
            CONVERT_VEC_TIME_DUR(pedals, new_t, track, helper);                                 \
            CONVERT_VEC_TIME(pitch_bends, new_t, track, helper);                                \
            CONVERT_VEC_TIME(controls, new_t, track, helper);                                   \
            new_s.tracks.emplace_back(std::move(new_t));                                        \
        }                                                                                       \
        return new_s;                                                                           \
    }

//                To    From
IMPLEMENT_CONVERT(Tick, Quarter)
IMPLEMENT_CONVERT(Quarter, Tick)

#undef IMPLEMENT_CONVERT
#undef CONVERT_VEC_TIME_DUR
#undef CONVERT_VEC_TIME
#undef HELPER_NAME

// Directly copy for the same type
template<> Score<Tick> convert<Tick, Tick> (const Score<Tick> & score) { return score; }
template<> Score<Quarter> convert<Quarter, Quarter> (const Score<Quarter> & score) { return score; }
template<> Score<Second> convert<Second, Second> (const Score<Second> & score) { return score; }

// Implementation of to_note_arr
template<TType T>
NoteArr<T> to_note_arr(const Track<T> & track) {
    NoteArr<T> note_arr{track.name, track.program, track.is_drum, track.note_num()};
    for (const auto & note: track.notes) {
        note_arr.push_back(note);
    }   return note_arr;
}

template<TType T>
Track<T> to_track(const NoteArr<T> & note_arr) {
    Track<T> track{note_arr.name, note_arr.program, note_arr.is_drum};
    const size_t note_num = note_arr.note_num();
    track.notes.reserve(note_num);
    for(size_t i = 0; i < note_num; ++i) {
        track.notes.emplace_back(
            note_arr.time[i],
            note_arr.duration[i],
            note_arr.pitch[i],
            note_arr.velocity[i]
        );
    }
    return track;
}

// Instantiation of to_note_arr and to_track
#define INSTANTIATE(__COUNT, T) \
    template NoteArr<T> to_note_arr(const Track<T> & track); \
    template Track<T> to_track(const NoteArr<T> & note_arr);

REPEAT_ON(INSTANTIATE, Tick, Quarter, Second)

#undef INSTANTIATE

} // namespace symusic