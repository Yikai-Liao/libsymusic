//
// Created by lyk on 23-12-25.
//
#include "symusic/track.h"
#include "symusic/ops.h"

namespace symusic {

template<TType T>
typename T::unit Track<T>::start() const {
    typename T::unit ans = std::numeric_limits<typename T::unit>::max();
    for (const auto & note: notes) {
        ans = std::min(ans, note.time);
    }   return ans;
}

template<TType T>
typename T::unit Track<T>::end() const {
    typename T::unit ans = std::numeric_limits<typename T::unit>::min();
    for (const auto & note: notes) {
        ans = std::max(ans, note.time + note.duration);
    }   return ans;
}

template<TType T>
size_t Track<T>::note_num() const {
    return notes.size();
}

template<TType T>
bool Track<T>::empty() const {
    return notes.empty() && controls.empty() && pitch_bends.empty() && pedals.empty();
}

template<TType T>
Track<T>& Track<T>::sort_inplace(const bool reverse) {
    ops::sort_by_time(notes, reverse);
    ops::sort_by_time(controls, reverse);
    ops::sort_by_time(pitch_bends, reverse);
    ops::sort_by_time(pedals, reverse);
    return *this;
}

template<TType T>
Track<T> Track<T>::sort(const bool reverse) {
    return copy().sort_inplace(reverse);
}

template<TType T>
Track<T> Track<T>::clip(unit start, unit end, bool clip_end) const {
    return {
        name, program, is_drum,
        std::move(ops::clip(notes, start, end, clip_end)),
        std::move(ops::clip(controls, start, end)),
        std::move(ops::clip(pitch_bends, start, end)),
        std::move(ops::clip(pedals, start, end, clip_end))
    };
}

#define INSTANTIATE_TRACK(__COUNT, T) template struct Track<T>;
REPEAT_ON(INSTANTIATE_TRACK, Tick, Quarter, Second)
#undef INSTANTIATE_TRACK

}
