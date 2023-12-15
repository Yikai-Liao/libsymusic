//
// Created by lyk on 23-12-14.
//
#ifndef LIBSYMUSIC_REPR_H
#define LIBSYMUSIC_REPR_H

// for ostream
#include <iosfwd>
#include "symusic/container.h"

// operator <<
template<symusic::trait::TimeEvent T>
std::ostream& operator<<(std::ostream& os, const T & event);

template<symusic::trait::TimeEvent T>
std::ostream& operator<<(std::ostream& os, const symusic::vec<T>& v);

#endif //LIBSYMUSIC_REPR_H
