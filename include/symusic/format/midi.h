//
// Created by lyk on 23-12-13.
//

#ifndef LIBSYMUSIC_MIDI_H
#define LIBSYMUSIC_MIDI_H


#include "symusic/format/common.h"
#include "MiniMidi.hpp"
namespace symusic {

namespace tag {
struct Midi: FormatTag<minimidi::file::MidiFile>{};
}

namespace details {
Score<tag::Tick> parse_midi_tick(std::span<u8> buffer);
Score<tag::Quarter> parse_midi_quarter(std::span<u8> buffer);
Score<tag::Second> parse_midi_second(std::span<u8> buffer);
}

// sepcify all the from template in header,
// so that container.h won't link to midi.cpp automatically

// from path
template<> template<> // a static function
inline Score<tag::Tick> Score<tag::Tick>::from<tag::Midi>(const std::span<u8> buffer) {
    return details::parse_midi_tick(buffer);
}

template<> template<> // a static function
inline Score<tag::Quarter> Score<tag::Quarter>::from<tag::Midi>(const std::span<u8> buffer) {
    return details::parse_midi_quarter(buffer);
}

template<> template<> // a static function
inline Score<tag::Second> Score<tag::Second>::from<tag::Midi>(const std::span<u8> buffer) {
    return details::parse_midi_second(buffer);
}
}   // symusic
#endif //LIBSYMUSIC_MIDI_H