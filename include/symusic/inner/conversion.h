//
// Created by lyk on 23-12-17.
//

#ifndef LIBSYMUSIC_INNER_CONVERSION_H
#define LIBSYMUSIC_INNER_CONVERSION_H
#include "symusic/container.h"

namespace symusic {

template<trait::TType From, trait::TType To>
Score<To> convert(const Score<From> & score);

}   // namespace symusic

#endif //LIBSYMUSIC_INNER_CONVERSION_H
