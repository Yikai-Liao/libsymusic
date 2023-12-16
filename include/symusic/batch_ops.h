//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_BATCH_OPS_H
#define LIBSYMUSIC_BATCH_OPS_H

#include <functional>
#include "symusic/alias.h"
#include "symusic/tag.h"

namespace symusic::ops {

// cmp function
template<typename T, class Compare>
void sort_branchless(const vec<T> & data, Compare cmp);

template<trait::TimeEvent T>
void sort_branchless(const vec<T> & data);

template<class Iter, class Compare>
void sort_branchless(Iter begin, Iter end, Compare comp);

template<typename T, class Compare>
void sort(vec<T>& data, Compare cmp);

template<class Iter, class Compare>
void sort(Iter begin, Iter end, Compare comp);

template<trait::TimeEvent T>
vec<T> filter(const vec<T>& events, std::function<bool(const T&)> t_fiter);

template<trait::TimeEvent T> // used for general events, with time
vec<T> clip(const vec<T>& events, typename T::unit start, typename T::unit end);

template<trait::TimeEvent T> // used for events with duration (e.g. Note and Pedal)
vec<T> clip(const vec<T>& events, typename T::unit start, bool clip_end);


}

#endif //LIBSYMUSIC_BATCH_OPS_H
