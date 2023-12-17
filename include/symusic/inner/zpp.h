//
// Created by lyk on 23-12-17.
//

#ifndef LIBSYMUSIC_INNER_ZPP_H
#define LIBSYMUSIC_INNER_ZPP_H

#include <span>
namespace symusic::io {

template<class T>
vec<u8> serailize_zpp(const T & data);

template<class T>
T deserailize_zpp(std::span<const u8> buffer);
}


#endif //ZPP_H
