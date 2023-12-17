//
// Created by lyk on 23-12-17.
//

#ifndef LIBSYMUSIC_CONVERSION_H
#define LIBSYMUSIC_CONVERSION_H
#include "symusic/inner/conversion.h"
namespace symusic {
extern template Score<tag::Tick> convert<tag::Tick, tag::Tick>(const Score<tag::Tick> & score);
extern template Score<tag::Tick> convert<tag::Quarter, tag::Tick>(const Score<tag::Quarter> & score);
// extern template Score<tag::Tick> convert<tag::Second, tag::Tick>(const Score<tag::Second> & score);

extern template Score<tag::Quarter> convert<tag::Tick, tag::Quarter>(const Score<tag::Tick> & score);
extern template Score<tag::Quarter> convert<tag::Quarter, tag::Quarter>(const Score<tag::Quarter> & score);
// extern template Score<tag::Quarter> convert<tag::Second, tag::Quarter>(const Score<tag::Second> & score);

// extern template Score<tag::Second> convert<tag::Tick, tag::Second>(const Score<tag::Tick> & score);
// extern template Score<tag::Second> convert<tag::Quarter, tag::Second>(const Score<tag::Quarter> & score);
extern template Score<tag::Second> convert<tag::Second, tag::Second>(const Score<tag::Second> & score);
}
#endif //LIBSYMUSIC_CONVERSION_H
