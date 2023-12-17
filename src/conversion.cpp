//
// Created by lyk on 23-12-16.
//
#include <cmath>
#include "symusic/inner/conversion.h"
namespace symusic {
namespace details {

struct Tick2Quarter {
    float tpq;

    template<trait::TType T>
    explicit Tick2Quarter(const Score<T> &score): tpq(static_cast<float>(score.ticks_per_quarter)) {}

    float operator()(const i32 tick) const {
        return static_cast<float>(tick) / tpq;
    }
};

struct Quarter2Tick {
    float tpq;

    template<trait::TType T>
    explicit Quarter2Tick(const Score<T> &score): tpq(static_cast<float>(score.ticks_per_quarter)) {}

    i32 operator()(const float quarter) const {
        return static_cast<i32>(std::round(quarter * tpq));
    }
};
}

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

#define IMPLEMENT_CONVERT(From, To) \
    template<> Score<tag::To> convert<tag::From, tag::To> (const Score<tag::From> & score) {    \
        HELPER_NAME(From, To) helper(score);                                                    \
        Score<tag::To> new_s(score.ticks_per_quarter);                                          \
        CONVERT_VEC_TIME(key_signatures, new_s, score, helper);                                 \
        CONVERT_VEC_TIME(time_signatures, new_s, score, helper);                                \
        CONVERT_VEC_TIME(tempos, new_s, score, helper);                                         \
        CONVERT_VEC_TIME(lyrics, new_s, score, helper);                                         \
        CONVERT_VEC_TIME(markers, new_s, score, helper);                                        \
        new_s.tracks.reserve(score.tracks.size());                                              \
        for(const Track<tag::From> &track: score.tracks) {                                      \
            Track<tag::To> new_t(track.name, track.program, track.is_drum);                     \
            CONVERT_VEC_TIME_DUR(notes, new_t, track, helper);                                  \
            CONVERT_VEC_TIME_DUR(pedals, new_t, track, helper);                                 \
            CONVERT_VEC_TIME(pitch_bends, new_t, track, helper);                                \
            CONVERT_VEC_TIME(controls, new_t, track, helper);                                   \
            new_s.tracks.emplace_back(std::move(new_t));                                        \
        }                                                                                       \
        return new_s;                                                                           \
    }

IMPLEMENT_CONVERT(Tick, Quarter)
IMPLEMENT_CONVERT(Quarter, Tick)

template<> Score<tag::Tick> convert<tag::Tick, tag::Tick> (const Score<tag::Tick> & score) { return score; }
template<> Score<tag::Quarter> convert<tag::Quarter, tag::Quarter> (const Score<tag::Quarter> & score) { return score; }
template<> Score<tag::Second> convert<tag::Second, tag::Second> (const Score<tag::Second> & score) { return score; }

}