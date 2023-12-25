//
// Created by lyk on 23-12-25.
//
#include "symusic/score.h"
#include "symusic/ops.h"

namespace symusic {

template<TType T>
typename T::unit Score<T>::start() const {
    typename T::unit ans = std::numeric_limits<typename T::unit>::max();
    for (const auto & track: tracks) {
        ans = std::min(ans, track.start());
    }   return ans;
}

template<TType T>
typename T::unit Score<T>::end() const {
    typename T::unit ans = std::numeric_limits<typename T::unit>::min();
    for (const auto & track: tracks) {
        ans = std::max(ans, track.end());
    }   return ans;
}

template<TType T>
size_t Score<T>::note_num() const {
    size_t ans = 0;
    for (const auto & track: tracks) {
        ans += track.note_num();
    }   return ans;
}

template<TType T>
bool Score<T>::empty() const {
    return tracks.empty()
        && time_signatures.empty()
        && key_signatures.empty()
        && tempos.empty()
        && lyrics.empty()
        && markers.empty();
}

template<TType T>
size_t Score<T>::track_num() const {
    return tracks.size();
}

template<TType T>
Score<T>& Score<T>::sort_inplace(const bool reverse) {

    for(auto &track: tracks) track.sort_inplace(reverse);

    ops::sort_by_time(time_signatures, reverse);
    ops::sort_by_time(key_signatures, reverse);
    ops::sort_by_time(tempos, reverse);
    ops::sort_by_time(lyrics, reverse);
    ops::sort_by_time(markers, reverse);

    return *this;
}

template<TType T>
Score<T> Score<T>::sort(const bool reverse) const {
    return copy().sort_inplace(reverse);
}

// #define EXTERN_TRACK_CLIP(__COUNT, T)   \
//     extern template Track<T> Track<T>::clip(typename T::unit start, typename T::unit end, bool clip_end) const;
//
// REPEAT_ON(EXTERN_TRACK_CLIP, Tick, Quarter, Second)
//
// #undef EXTERN_TRACK_CLIP

template<TType T>
Score<T> Score<T>::clip(unit start, unit end, bool clip_end) const {
    vec<Track<T>> new_tracks;
    new_tracks.reserve(track_num());
    for(const auto & track: tracks) {
        new_tracks.emplace_back(track.clip(start, end, clip_end));
    }

    return {
        ticks_per_quarter,
        std::move(new_tracks),
        std::move(ops::clip(time_signatures, start, end)),
        std::move(ops::clip(key_signatures, start, end)),
        std::move(ops::clip(tempos, start, end)),
        std::move(ops::clip(lyrics, start, end)),
        std::move(ops::clip(markers, start, end))
    };
}

#define INSTANTIATE_SCORE(__COUNT, T) template struct Score<T>;
REPEAT_ON(INSTANTIATE_SCORE, Tick, Quarter, Second)
#undef INSTANTIATE_SCORE

}