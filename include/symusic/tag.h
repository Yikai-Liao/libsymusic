//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_TAG_H
#define LIBSYMUSIC_TAG_H

#include "symusic/mtype.h"

namespace symusic {

namespace tag {
// Time Unit Base Class
template<typename T> requires std::is_arithmetic_v<T>
struct TimeUnit { typedef T unit; };

// Specific Time Units
struct Tick: TimeUnit<i32> {};

struct Quarter: TimeUnit<f32> {};

struct Second: TimeUnit<f32> {};

struct FormatTag{};

struct MIDI: FormatTag{};

struct XML: FormatTag{};

struct ZPP: FormatTag{};

} // namespace tag

namespace trait {
// define concept for TimeUnit
template<typename T>
concept TType = std::is_base_of_v<tag::TimeUnit<typename T::unit>, T>;

template<typename T>
concept Format = std::is_base_of_v<tag::FormatTag, T>;

}   // namespace trait

// Define TimeStamp for events like Note
template<trait::TType T>
struct TimeStamp {
    typedef T ttype;
    typedef typename T::unit unit;
    unit time;

    TimeStamp() = default;
    TimeStamp(const TimeStamp &) = default;
    explicit TimeStamp(const unit &time) : time(time) {};
};

namespace trait {
// define concept for TimeStamp
template<typename T>
concept TimeEvent = std::is_base_of_v<TimeStamp<typename T::ttype>, T>;

}   // namespace trait

}   // namespace symusic

#endif //LIBSYMUSIC_TAG_H
