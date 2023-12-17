//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_BATCH_OPS_H
#define LIBSYMUSIC_BATCH_OPS_H

#include <functional>
#include "symusic/tag.h"
#include "pdqsort.h"
#include "algorithm"

namespace symusic::ops {

// cmp function
template<typename T, class Compare>
void sort_branchless(vec<T> & data, Compare cmp){
    pdqsort_branchless(data.begin(), data.end(), cmp);
}

template<typename T>
void sort_by_time(vec<T> & data) {
    std::sort(data.begin(), data.end(),
        [](const T & a, const T & b) {return (a.time) < (b.time);}
    );
}

template<class Iter, class Compare>
void sort_branchless(Iter begin, Iter end, Compare cmp) {
    pdqsort_branchless(begin, end, cmp);
}

template<typename T, class Compare>
void sort(vec<T>& data, Compare cmp){
    pdqsort(data.begin(), data.end(), cmp);
}

template<class Iter, class Compare>
void sort(Iter begin, Iter end, Compare cmp) {
    pdqsort(begin, end, cmp);
}

template<class T, class FILTER>
vec<T> filter(const vec<T>& data, FILTER t_fiter) {
    if (data.empty()) return {};
    vec<T> new_data;
    new_data.reserve(data.size());
    std::copy_if(
        data.begin(), data.end(),
        std::back_inserter(new_data), t_fiter
    );
    new_data.shrink_to_fit();
    return new_data;
}

template<trait::TimeEvent T> // used for general events, with time
vec<T> clip(const vec<T>& events, typename T::unit start, typename T::unit end) {
    auto func = [start, end](const T &event) {
        return ((event.time) >= start) && ((event.time) < end);
    };  return filter(events, func);
}

template<trait::TimeEvent T> // used for events with duration (e.g. Note and Pedal)
vec<T> clip(const vec<T>& events, typename T::unit start, typename T::unit end, const bool clip_end) {
    if (clip_end) {
        auto func = [start, end](const T &event) {
            return (event.time >= start) && (event.time + event.duration <= end);
        };  return filter(events, func);
    }   return clip(events, start, end);
}
}

#endif //LIBSYMUSIC_BATCH_OPS_H