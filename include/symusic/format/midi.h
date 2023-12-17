//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_MIDI_H
#define LIBSYMUSIC_MIDI_H

#include "symusic/inner/io.h"
#include "MiniMidi.hpp"
namespace symusic::io {

extern template Score<tag::Tick> parse<tag::Tick, tag::MIDI>(std::span<const u8> buffer);
extern template Score<tag::Quarter> parse<tag::Quarter, tag::MIDI>(std::span<const u8> buffer);

}
#endif //LIBSYMUSIC_MIDI_H
