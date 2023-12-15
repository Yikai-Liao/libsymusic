//
// Created by lyk on 23-12-13.
//
#include <stdexcept>
#include "symusic/container.h"

namespace symusic {

namespace details {
inline i8 safe_add(const i8 a, const i8 b) {
    const int ans = a + b;
    if (ans > 127 || ans < 0)
        throw std::range_error("Overflow while adding " + std::to_string(a) + " and " + std::to_string(b));
    return static_cast<i8>(ans);
}
}

template<trait::TType T>
inline Note<T>& Note<T>::shift_pitch_inplace(i8 offset) {
    pitch = details::safe_add(pitch, offset);
    return *this;
}

template<trait::TType T>
inline Note<T>& Note<T>::shift_velocity_inplace(i8 offset) {
    velocity = details::safe_add(velocity, offset);
    return *this;
}

template<trait::TType T>
inline bool Note<T>::operator==(const Note<T> & other) const {
    return this->time == other.time
        && duration == other.duration
        && pitch == other.pitch
        && velocity == other.velocity;
}



}   // namespace symusic