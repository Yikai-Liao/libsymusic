//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_BATCH_OPS_H
#define LIBSYMUSIC_BATCH_OPS_H

#include <functional>
#include "symusic/alias.h"
#include "symusic/tag.h"

namespace symusic {

namespace ops {

// cmp function
template<trait::TimeEvent T>
void sort(
    const vec<T> & events,
    std::function<bool(const T&, const T&)> cmp =
        [](const T& a, const T& b) { return (a.time) < (b.time); } // () is for making clion happy
);

template<trait::TimeEvent T>
vec<T> filter(const vec<T>& events, std::function<bool(const T&)> t_fiter);

template<trait::TimeEvent T> // used for general events, with time
vec<T> clip(const vec<T>& events, typename T::unit start, typename T::unit end);

template<trait::TimeEvent T> // used for events with duration (e.g. Note and Pedal)
vec<T> clip(const vec<T>& events, typename T::unit start, bool clip_end);


}
}
#endif //LIBSYMUSIC_BATCH_OPS_H
