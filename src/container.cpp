//
// Created by lyk on 23-12-13.
//
#include <stdexcept>
#include <limits>
#include "symusic/inner/container.h"
#include "MetaMacro.h"

namespace symusic {

// Implementation of NoteArray

template<trait::TType T>
NoteArray<T>::NoteArray(std::string name, const u8 program, const bool is_drum, const vec<Note<T>> & notes):
    name{std::move(name)}, program{program}, is_drum{is_drum} {
    size_t note_num = notes.size();

    time.reserve(note_num);
    duration.reserve(note_num);
    pitch.reserve(note_num);
    velocity.reserve(note_num);

    for(const auto & note: notes) {
        time.emplace_back(note.time);
        duration.emplace_back(note.duration);
        pitch.emplace_back(note.pitch);
        velocity.emplace_back(note.velocity);
    }
}
template<trait::TType T>
void check(const NoteArray<T> & array) {
    size_t num = array.time.size();
    if(!(num == array.duration.size() && num == array.pitch.size() && num == array.velocity.size())) {
        throw std::runtime_error("NoteArray: size of time, duration, pitch and velocity are not equal");
    }
}

template<trait::TType T>
typename NoteArray<T>::unit NoteArray<T>::start() const {
    check(*this);
    if (time.empty()) {
        throw std::runtime_error("NoteArray is empty");
    }
    unit ans = std::numeric_limits<unit>::max();
    for (const auto & t: time) {
        ans = std::min(ans, t);
    }   return ans;
}

template<trait::TType T>
typename NoteArray<T>::unit NoteArray<T>::end() const {
    check(*this);
    if (time.empty()) {
        throw std::runtime_error("NoteArray is empty");
    }
    unit ans = std::numeric_limits<unit>::min();
    for (const auto & t: time) {
        ans = std::max(ans, t);
    }   return ans;
}

template<trait::TType T>
size_t NoteArray<T>::note_num() const {
    check(*this);
    return time.size();
}

template<trait::TType T>
bool NoteArray<T>::empty() const {
    check(*this);
    return time.empty();
}

// Implementation of Track
template<trait::TType T>
typename T::unit Track<T>::start() const {
    if (notes.empty()) {
        throw std::runtime_error("Track got no notes");
    }
    typename T::unit ans = std::numeric_limits<typename T::unit>::max();
    for (const auto & note: notes) {
        ans = std::min(ans, note.time);
    }   return ans;
}

template<trait::TType T>
typename T::unit Track<T>::end() const {
    if (notes.empty()) {
        throw std::runtime_error("Track got no notes");
    }
    typename T::unit ans = std::numeric_limits<typename T::unit>::min();
    for (const auto & note: notes) {
        ans = std::max(ans, note.time + note.duration);
    }   return ans;
}

template<trait::TType T>
size_t Track<T>::note_num() const {
    return notes.size();
}

template<trait::TType T>
bool Track<T>::empty() const {
    return notes.empty() && controls.empty() && pitch_bends.empty() && pedals.empty();
}

// Implementation of Score
template<trait::TType T>
typename T::unit Score<T>::start() const {
    if (tracks.empty()) {
        throw std::runtime_error("Score got no tracks");
    }
    typename T::unit ans = std::numeric_limits<typename T::unit>::max();
    for (const auto & track: tracks) {
        ans = std::min(ans, track.start());
    }   return ans;
}

template<trait::TType T>
typename T::unit Score<T>::end() const {
    if (tracks.empty()) {
        throw std::runtime_error("Score got no tracks");
    }
    typename T::unit ans = std::numeric_limits<typename T::unit>::min();
    for (const auto & track: tracks) {
        ans = std::max(ans, track.end());
    }   return ans;
}

template<trait::TType T>
size_t Score<T>::note_num() const {
    size_t ans = 0;
    for (const auto & track: tracks) {
        ans += track.note_num();
    }   return ans;
}

template<trait::TType T>
size_t Score<T>::track_num() const {
    return tracks.size();
}

template<trait::TType T>
bool Score<T>::empty() const {
    return tracks.empty() && time_signatures.empty() && key_signatures.empty() && tempos.empty();
}

// explicit instantiation of all the classes
#define INSTANTIATE(_COUNT, T)          \
    template struct T<tag::Tick>;       \
    template struct T<tag::Quarter>;    \
    template struct T<tag::Second>;

REPEAT_ON(
    INSTANTIATE,
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

#undef INSTANTIATE
}   // namespace symusic